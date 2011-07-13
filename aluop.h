#ifndef _ALUOP_H
#define _ALUOP_H

#include "inst.h"
#include "env.h"

extern int (*alu_reg_op[16])(struct CPUState *, uint32_t);
extern int (*alu_imm_op[16])(struct CPUState *, uint32_t);

static inline int sflag(uint32_t inst)
{
    return (inst & SFLAG_MASK) >> SFLAG_SHIFT;
}

static inline uint32_t getrd(uint32_t inst)
{
    return (inst & RD_MASK) >> RD_SHIFT;
}

static inline uint32_t getrm(uint32_t inst)
{
    return (inst & RM_MASK) >> RM_SHIFT;
}

static inline uint32_t getrn(uint32_t inst)
{
    return (inst & RN_MASK) >> RN_SHIFT;
}

static inline uint32_t getimm12(uint32_t inst)
{
    return (inst & IMM12_MASK);
}

static inline uint32_t getimm5(uint32_t inst)
{
    return (inst & IMM5_MASK) >> IMM5_SHIFT;
}

static inline uint32_t gettype(uint32_t inst)
{
    return (inst & TYPE_MASK) >> TYPE_SHIFT;
}

#endif

