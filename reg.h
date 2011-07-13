#ifndef _REG_H
#define _REG_H

#include "env.h"

char *reg_name(int reg);
uint32_t get_reg(struct CPUState *env, int reg);
void set_reg(struct CPUState *env, int reg, uint32_t val);

#endif

