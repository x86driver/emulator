#ifndef _COND_H
#define _COND_H

#include <stdint.h>
#include "env.h"

const char *cond_name(uint32_t cond);
int check_cond(struct CPUState *env, uint32_t inst);

enum {
    COND_EQ = 0,
    COND_NE,
    COND_CS,
    COND_CC,
    COND_MI,
    COND_PL,
    COND_VS,
    COND_VC,
    COND_HI,
    COND_LS,
    COND_GE,
    COND_LT,
    COND_GT,
    COND_LE,
    COND_AL
};

#endif
