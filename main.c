#define _BSD_SOURCE
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <endian.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "inst.h"
#include "load_store.h"
#include "env.h"
#include "aluop.h"
#include "function.h"
#include "branch.h"
#include "reg.h"

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
    bit[4] = getbit(inst, BIT4);

    if (bit[25] == 0) { /* register */
        if (bit[24] == 1 && bit[23] == 0 && bit[20] == 0) {
        } else if (bit[4] == 0) {
            aluop = (inst & ALU_OP_MASK) >> ALU_OP_SHIFT;
            alu_reg_op[aluop](env, inst);
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
    if (getbit(inst, BIT25) == 1 && getbit(inst, BIT24) == 0)
        branch(env, inst);
    else
        derror("Unsupport instruction on 0x%x\n", env->pc);

    return 0;
}

uint32_t fetch_inst(struct CPUState *env)
{
    return env->memory[env->pc / 4];
}

int main(int argc, char **argv)
{
    int fd, c;
    ssize_t ret;
    uint32_t inst;
    struct CPUState *env;

    env = malloc(sizeof(struct CPUState));
    if (!env) {
        perror("malloc");
        exit(1);
    }
    memset(env, 0, sizeof(struct CPUState));

    fd = open("a.bin", O_RDONLY);
    ret = read(fd, &env->memory, 4096);

    while (env->pc <= ret) {
        inst = fetch_inst(env);
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
        default:
            printf("undefined instruction %x\n", inst);
            break;
        }
        next_pc(env);
    }

    close(fd);

    while ((c = getopt(argc, argv, "r")) != -1) {
        switch (c) {
        case 'r':
            dump_reg(env);
            break;
        }
    }
    return 0;
}
