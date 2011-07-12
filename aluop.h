#ifndef _ALUOP_H
#define _ALUOP_H

#include "inst.h"

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

#endif

