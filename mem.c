#include <stdio.h>
#include "mem.h"

#define MEM_SIZE 4096

static uint32_t memory[MEM_SIZE];

uint32_t get_mem(uint32_t addr)
{
    return memory[addr];
}

uint8_t get_mem_byte(uint32_t addr)
{
    return memory[addr] & 0x0ff;
}
