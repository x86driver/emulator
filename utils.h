#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <stdint.h>
#include "env.h"
#include "aluop.h"
#include "cond.h"

static inline void print_preamble(struct CPUState *env, uint32_t inst)
{
    printf("%4x:\t%08x \t", env->pc, inst);
    env->pc += 4;
}

static inline void print_inst(const char *name, uint32_t inst)
{
    printf("%s%s%s", name, sflag(inst) ? "s" : "", cond_name(inst));
}

static inline void print_inst_ldst(const char *name, uint32_t inst)
{
    uint32_t B = getbit(inst, BIT22);
    printf("%s%s%s", name, (B ? "b" : ""), cond_name(inst));
}

#endif
