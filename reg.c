#include <stdint.h>
#include "env.h"
#include "reg.h"

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

uint32_t get_reg(struct CPUState *env, int reg)
{
    return env->regs[reg];
}

void set_reg(struct CPUState *env, int reg, uint32_t val)
{
    if (reg == REG_PC)
        set_pc(env, val);
    else
        env->regs[reg] = val;
}

void set_pc(struct CPUState *env, uint32_t imm32)
{
    env->regs[REG_PC] = imm32 + 4;  /* on next_pc() will plus 4 */
    env->pc = (imm32/4)-1;
}
