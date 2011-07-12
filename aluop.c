#include <stdio.h>
#include <stdint.h>
#include "inst.h"
#include "aluop.h"

void print_preamble(inst)
{
    static int count = 0;
    printf("%4x:\t%x\t", count, inst);
    count += 4;
}

char *regname(int reg)
{
    static char *reg_name[] = {
        "r0", "r1", "r2", "r3",
        "r4", "r5", "r6", "r7",
        "r8", "r9", "sl", "fp",
        "ip", "sp", "lr", "pc"
    };

    return reg_name[reg];
}

int no_op(uint32_t inst)
{
    return 0;
}

int mov_reg(uint32_t inst)
{
    uint32_t rd = getrd(inst);
    uint32_t rm = getrm(inst);

    print_preamble(inst);
    if (sflag(inst))
        printf("movs\t%s, %s\n", regname(rd), regname(rm));
    else
        printf("mov\t%s, %s\n", regname(rd), regname(rm));

    return 0;
}

int add_reg(uint32_t inst)
{
    uint32_t rd = getrd(inst);
    uint32_t rm = getrm(inst);
    uint32_t rn = getrn(inst);

    print_preamble(inst);
    printf("add\t%s, %s, %s\n", regname(rd), regname(rn), regname(rm));

    return 0;
}

int mov_imm(uint32_t inst)
{
    uint32_t rd = getrd(inst);
    uint32_t imm12 = (inst & IMM12_MASK);

    print_preamble(inst);
    printf("mov\t%s, #%d\t; 0x%x\n", regname(rd), imm12, imm12);

    return 0;
}

int (*alu_reg_op[16])(uint32_t) = {
    no_op, no_op, no_op, no_op,
    add_reg, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op,
    no_op, mov_reg, no_op, no_op
};

int (*alu_imm_op[16])(uint32_t) = {
    no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op,
    no_op, mov_imm, no_op, no_op
};
