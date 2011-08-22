#include "inst.h"
#include "reg.h"
#include "misc.h"

static inline uint32_t getop2(uint32_t inst)
{
    return (inst & 0x0f0) >> 4;
}

int bx_reg(struct CPUState *env, uint32_t inst)
{
    uint32_t rm = getrm(inst);

    set_reg(env, REG_PC, get_reg(env, rm));

    return 0;
}

int clz_reg(struct CPUState *env, uint32_t inst)
{
    uint32_t rd = getrd(inst);
    uint32_t rm = getrm(inst);
    uint32_t bits = __builtin_clz(get_reg(env, rm));

    set_reg(env, rd, bits);

    return 0;
}

int misc_reg_inst(struct CPUState *env, uint32_t inst)
{
    uint32_t op2 = getop2(inst);

    switch (op2) {
    case 0:
        if (getbit(inst, BIT21) == 0) {
            mrs_reg(env, inst);
        } else {
            printf("Unsupport instruction @ 0x%x\n", get_reg(env, REG_PC));
        }
        break;
    case 1:
        if (getbit(inst, BIT22) == 0)
            bx_reg(env, inst);
        else
            clz_reg(env, inst);
        break;
    default:
        printf("Unsupport instruction @ 0x%x\n", get_reg(env, REG_PC));
        break;
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
