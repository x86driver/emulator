#include <stdio.h>
#include <stdint.h>
#include "inst.h"
#include "env.h"
#include "utils.h"
#include "reg.h"

uint32_t sign_extend_24(uint32_t imm24)
{
    if (imm24 & 0x00800000)
        return (imm24 | 0xff000000);
    return imm24;
}

int branch(struct CPUState *env, uint32_t inst)
{
    uint32_t imm24;
    uint32_t L = getbit(inst, BIT24);

    imm24 = getimm24(inst);
    imm24 = sign_extend_24(imm24);
    imm24 = (imm24 << 2) + get_reg(env, REG_PC);

    if (L) {
        set_reg(env, REG_LR, get_reg(env, REG_PC) - BRANCH_PC_OFFSET);
    }

    set_pc(env, imm24);

    return 0;
}
