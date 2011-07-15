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

    print_preamble(env, inst);
    print_inst_without_s("b", inst);
    printf("\t");
    print_branch_pc(env, inst);
    printf("\n");

    if (!check_cond(env, inst))
        return 0;
    imm24 = getimm24(inst);
    imm24 = sign_extend_24(imm24);
    imm24 = (imm24 << 2) + OFFSET_PC;
    set_pc(env, imm24);

    return 0;
}
