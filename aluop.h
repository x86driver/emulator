#ifndef _ALUOP_H
#define _ALUOP_H

#include "inst.h"
#include "env.h"

extern int (*alu_reg_op[16])(struct CPUState *, uint32_t);
extern int (*alu_imm_op[16])(struct CPUState *, uint32_t);

#endif

