#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "env.h"
#include "inst.h"
#include "aluop.h"
#include "cond.h"

const char *cond_name(uint32_t cond)
{
    static const char *name[] = {
        "eq", "ne", "cs", "cc",
        "mi", "pl", "vs", "vc",
        "hi", "ls", "ge", "lt",
        "gt", "le", ""
    };

    return name[getcond(cond)];
}

int check_cond(struct CPUState *env, uint32_t inst)
{
    uint32_t cond = getcond(inst);
    struct Reg_CPSR *cpsr = &env->cpsr;

    if (cond == COND_AL)
        return 1;

    switch (cond) {
    case COND_EQ:
        if (cpsr->Z == 1)
            return 1;
        else
            return 0;
    case COND_NE:
        if (cpsr->Z == 0)
            return 1;
        else
            return 0;
    case COND_CS:
        if (cpsr->C == 1)
            return 1;
        else
            return 0;
    case COND_MI:
        if (cpsr->N == 1)
            return 1;
        else
            return 0;
    case COND_PL:
        if (cpsr->N == 0)
            return 1;
        else
            return 0;
    case COND_VS:
        if (cpsr->V == 1)
            return 1;
        else
            return 0;
    case COND_VC:
        if (cpsr->V == 0)
            return 1;
        else
            return 0;
    case COND_HI:
        if (cpsr->C == 1 && cpsr->Z == 0)
            return 1;
        else
            return 0;
    case COND_LS:
        if (cpsr->C == 0 || cpsr->Z == 1)
            return 1;
        else
            return 0;
    case COND_GE:
        if (cpsr->N == cpsr->V)
            return 1;
        else
            return 0;
    case COND_LT:
        if (cpsr->N != cpsr->V)
            return 1;
        else
            return 0;
    case COND_GT:
        if (cpsr->Z == 0 && cpsr->N == cpsr->V)
            return 1;
        else
            return 0;
    case COND_LE:
        if (cpsr->Z == 1 || cpsr->N != cpsr->V)
            return 1;
        else
            return 0;
    default:
        derror("Undefined condition\n");
        exit(1);
    }
}
