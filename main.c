#define _BSD_SOURCE
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "inst.h"
#include "load_store.h"
#include "env.h"
#include "aluop.h"
#include "function.h"
#include "branch.h"
#include "reg.h"
#include "mem.h"
#include "super.h"
#include "misc.h"
#include "disas-arm/disas.h"

int inst_class(uint32_t inst)
{
    /* FIXME:
     * Remember cond bit
     */
    uint32_t bit[32];

    bit[27] = getbit(inst, BIT27);
    bit[26] = getbit(inst, BIT26);

    if (bit[27] == 0 && bit[26] == 0) {
        return CLASS_DATA_PROCESSING;
    }

    if (bit[27] == 0 && bit[26] == 1) {
        return CLASS_LOAD_STORE;
    }

    if (bit[27] == 1 && bit[26] == 0) {
        return CLASS_BRANCH;
    }

    if (bit[27] == 1 && bit[26] == 1) {
        return CLASS_SUPER;
    }

    return 0;
}

int dp_class(struct CPUState *env, uint32_t inst)
{
    uint32_t bit[32];
    uint32_t aluop;

    bit[25] = getbit(inst, BIT25);
    bit[24] = getbit(inst, BIT24);
    bit[23] = getbit(inst, BIT23);
    bit[20] = getbit(inst, BIT20);
    bit[7] = getbit(inst, BIT7);
    bit[4] = getbit(inst, BIT4);

    if (bit[25] == 0) { /* register */
        if (bit[24] == 1 && bit[23] == 0 && bit[20] == 0) { /* FIXME: may cause problem */
            if (bit[7] == 0) {
                misc_reg_inst(env, inst);
            }
        } else if (bit[4] == 0) {
            aluop = (inst & ALU_OP_MASK) >> ALU_OP_SHIFT;
            alu_reg_op[aluop](env, inst);
        } else if ((inst & 0x90) == 0x10) {
            aluop = (inst & ALU_OP_MASK) >> ALU_OP_SHIFT;
            alu_shift_op[aluop](env, inst);
        }
    } else {    /* immediate */
        if (bit[24] == 1 && bit[23] == 0 && bit[20] == 0) {
        } else {
            aluop = (inst & ALU_OP_MASK) >> ALU_OP_SHIFT;
            alu_imm_op[aluop](env, inst);
        }
    }

    return 0;
}

int load_store_class(struct CPUState *env, uint32_t inst)
{
    if (getbit(inst, BIT25) == 0)
        ldst_imm(env, inst);
    else
        ldst_reg(env, inst);
    return 0;
}

int branch_class(struct CPUState *env, uint32_t inst)
{
    if (getbit(inst, BIT25) == 1) {
        branch(env, inst);
    } else {
        multi_ldst(env, inst);
    }

    return 0;
}

int super_class(struct CPUState *env, uint32_t inst)
{
    if ((inst & 0x0f000000) == 0x0f000000)
        return svc(env, inst);

    return 0;
}

int decode_inst(struct CPUState *env, uint32_t inst)
{
    int ret = 0;

    if (!check_cond(env, inst))
        return 0;
    switch (inst_class(inst)) {
    case CLASS_DATA_PROCESSING:
        dp_class(env, inst);
        break;
    case CLASS_LOAD_STORE:
        load_store_class(env, inst);
        break;
    case CLASS_BRANCH:
        branch_class(env, inst);
        break;
    case CLASS_SUPER:
        ret = super_class(env, inst);
        break;
    default:
        printf("undefined instruction %x\n", inst);
        break;
    }

    return ret;
}

uint32_t fetch_inst(struct CPUState *env)
{
    return get_pc_mem(env, env->pc);
}

int check_elf(const char *ptr)
{
    if (ptr[1] == 'E' && ptr[2] == 'L' && ptr[3] == 'F')
        return 1;
    else
        return 0;
}

int parse_file(struct CPUState *env, const char *file_name)
{
    char *ptr;
    struct stat st;
    int fd = open(file_name, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    fstat(fd, &st);
    ptr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return -2;
    }

    if (check_elf(ptr)) {   /* ELF file */
        memcpy(env->memory+(0x8000/4), ptr, st.st_size);
        uint32_t entry = get_mem(env, 0x8018);
        printf("Entry point: 0x%x\n", entry);
        set_pc(env, entry+4);
        set_reg(env, REG_SP, 0x4007f2c8);
    } else {    /* raw file */
        memcpy(env->memory, ptr, st.st_size);
    }

    munmap(ptr, st.st_size);
    close(fd);
    return 0;
}

int main(int argc, char **argv)
{
    int c, ret;
    uint32_t inst;
    uint32_t dump_flag = 0;
    struct CPUState *env;
    char *file_name = NULL;

    env = malloc(sizeof(struct CPUState));
    if (!env) {
        perror("malloc");
        exit(1);
    }
    init_cpu_state(env);

    while ((c = getopt(argc, argv, "rdmf:")) != -1) {
        switch (c) {
        case 'r':
            dump_flag |= DUMP_REG;
            break;
        case 'd':
            dump_flag |= DUMP_CODE;
            break;
        case 'm':
            dump_flag |= DUMP_MEM;
            break;
        case 'f':
            file_name = optarg;
            break;
        }
    }

    if (file_name == NULL) {
        printf("No input file, use default a.bin\n");
        file_name = "a.bin";
    }

    printf("Open file: %s\n", file_name);
    parse_file(env, file_name);

    while (1) {
        inst = fetch_inst(env);
        ret = decode_inst(env, inst);
        if (ret)
            break;
        next_pc(env);
    }

    if (dump_flag & DUMP_CODE)
        disas(stdout, env->memory, (env->pc+1)*4);
    if (dump_flag & DUMP_REG)
        dump_reg(env);
    if (dump_flag & DUMP_MEM)
        dump_mem(env->memory, 0, 64);

    clean_cpu_state(env);
    free(env);

    return 0;
}
