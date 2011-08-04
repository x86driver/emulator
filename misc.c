#include "inst.h"
#include "reg.h"
#include "misc.h"

static inline uint32_t getop2(uint32_t inst)
{
    return (inst & 0x0f0);  /* We don't shift, just check if zero or not zero */
}

int misc_reg_inst(struct CPUState *env, uint32_t inst)
{
    uint32_t op2 = getop2(inst);

    if (op2 == 0) {
        if (getbit(inst, BIT21) == 0) {
            mrs_reg(env, inst);
        } else {
            printf("Unsupport instruction @ 0x%x\n", get_reg(env, REG_PC));
        }
    } else {
        printf("Unsupport instruction @ 0x%x\n", get_reg(env, REG_PC));
    }

    return 0;
}

int mrs_reg(struct CPUState *env, uint32_t inst)
{
    uint32_t rd = getrd(inst);
    int mode = get_mode(env);

    if (mode == MODE_USR) {
        set_reg(env, rd, get_cpsr(env));
    } else {
        printf("Unsupport mode @ 0x%x\n", get_reg(env, REG_PC));
    }

    return 0;
}
