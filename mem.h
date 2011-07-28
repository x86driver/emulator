#ifndef _MEM_H
#define _MEM_H

#include <stdint.h>
#include "env.h"

uint32_t get_mem(struct CPUState *env, uint32_t addr);
uint8_t get_mem_byte(struct CPUState *env, uint32_t addr);
uint32_t get_pc_mem(struct CPUState *env, uint32_t pc);
void set_mem(struct CPUState *env, uint32_t addr, uint32_t val);
void set_mem_byte(struct CPUState *env, uint32_t addr, uint8_t val);

#endif
