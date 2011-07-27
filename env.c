#include <string.h>
#include "env.h"
#include "reg.h"

void init_cpu_state(struct CPUState *env)
{
    memset(env, 0, sizeof(struct CPUState));
    set_reg(env, REG_PC, 8);
}
