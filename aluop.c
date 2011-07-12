#include <stdio.h>
#include <stdint.h>
#include "inst.h"

int no_op(uint32_t inst)
{
    return 0;
}

int mov_reg(uint32_t inst)
{
    uint32_t rd = (inst & RD_MASK) >> RD_SHIFT;
    uint32_t rm = (inst & RM_MASK) >> RM_SHIFT;

    printf("mov r%d, r%d\n", rd, rm);

    return 0;
}

int (*alu_reg_op[16])(uint32_t) = {
    no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op,
    no_op, mov_reg, no_op, no_op
};
