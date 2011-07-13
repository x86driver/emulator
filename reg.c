#include <stdint.h>

#define REG_NUM 17  /* r0-r15, cpsr */

static uint32_t regs[REG_NUM];

char *reg_name(int reg)
{
    static char *regname[] = {
        "r0", "r1", "r2", "r3",
        "r4", "r5", "r6", "r7",
        "r8", "r9", "sl", "fp",
        "ip", "sp", "lr", "pc"
    };

    return regname[reg];
}

uint32_t get_reg(int reg)
{
    return regs[reg];
}

void set_reg(int reg, uint32_t val)
{
    regs[reg] = val;
}
