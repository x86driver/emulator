#include <stdio.h>
#include "mem.h"
#include "env.h"

uint32_t get_mem(struct CPUState *env, uint32_t addr)
{
    return env->memory[addr];
}

uint8_t get_mem_byte(struct CPUState *env, uint32_t addr)
{
    return get_mem(env, addr) & 0x0ff;
}

void set_mem(struct CPUState *env, uint32_t addr, uint32_t val)
{
    env->memory[addr] = val;
}

void set_mem_byte(struct CPUState *env, uint32_t addr, uint8_t val)
{
    uint32_t tmp = get_mem(env, addr);
    set_mem(env, addr, (tmp & 0xffffff00) & (val & 0x0ff));
}
