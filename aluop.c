#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "inst.h"
#include "aluop.h"
#include "reg.h"
#include "env.h"

int no_op(struct CPUState *env, uint32_t inst)
{
    return 0;
}

int mov_reg(struct CPUState *env, uint32_t inst)
{
    uint32_t rd = getrd(inst);
    uint32_t rm = getrm(inst);
    uint32_t imm5 = getimm5(inst);
    uint32_t type = gettype(inst);

    print_preamble(env, inst);

    /* 在這裡判斷五種: mov, lsl, lsr, asr, rrx, ror */

    switch (type) {
    case 0: /* mov or lsl */
        if (imm5) { /* lsl */
            printf("Unfinished lsl\n");
            exit(1);
        } else {    /* mov */
            printf("mov");
            set_reg(env, rd, get_reg(env, rm));
            if (sflag(inst) && rd != REG_PC) {
                printf("s");
                env->cpsr.N = getbit(inst, BIT31);
                env->cpsr.Z = (rm == 0);
            }
        }
        break;
    default:
        printf("Unfinished mov shift\n");
        exit(1);
    }

    printf("\t%s, %s\n", reg_name(rd), reg_name(rm));

    return 0;
}

int add_reg(struct CPUState *env, uint32_t inst)
{
    uint32_t rd = getrd(inst);
    uint32_t rm = getrm(inst);
    uint32_t rn = getrn(inst);

    print_preamble(env, inst);
    printf("add\t%s, %s, %s\n", reg_name(rd), reg_name(rn), reg_name(rm));

    return 0;
}

int mov_imm(struct CPUState *env, uint32_t inst)
{
    uint32_t rd = getrd(inst);
    uint32_t imm12 = (inst & IMM12_MASK);

    print_preamble(env, inst);
    if (imm12 > 32)
        printf("mov\t%s, #%d\t; 0x%x\n", reg_name(rd), imm12, imm12);
    else
        printf("mov\t%s, #%d\n", reg_name(rd), imm12);

    set_reg(env, rd, imm12);
    if (sflag(inst) && rd != REG_PC) {
        env->cpsr.N = getbit(inst, BIT31);
        env->cpsr.Z = (rd == 0);
//        env->cpsr.C = doremi
    }

    return 0;
}

int (*alu_reg_op[16])(struct CPUState *, uint32_t) = {
    no_op, no_op, no_op, no_op,
    add_reg, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op,
    no_op, mov_reg, no_op, no_op
};

int (*alu_imm_op[16])(struct CPUState *, uint32_t) = {
    no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op,
    no_op, mov_imm, no_op, no_op
};
