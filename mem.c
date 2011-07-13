#include <stdio.h>
#include "mem.h"
#include "env.h"

uint32_t get_mem(struct CPUState *env, uint32_t addr)
{
    return env->memory[addr];
}

uint8_t get_mem_byte(struct CPUState *env, uint32_t addr)
{
    return env->memory[addr] & 0x0ff;
}
