#include <stdio.h>
#include "super.h"
#include "reg.h"

int svc(struct CPUState *env, uint32_t inst)
{
    env = env;
    inst = inst;
    printf("\nEnd simulation @ 0x%x\n\n", get_reg(env, REG_PC));

    return 1;
}
