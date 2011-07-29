#include <stdio.h>
#include "mem.h"
#include "env.h"
#include "inst.h"

static inline uint32_t get_phys_mem(struct CPUState *env, uint32_t phys_addr)
{
    if (phys_addr >= MEM_SIZE) {
        derror("read memory @ 0x%x failed\n", addr);
        return 0;
    }

    return env->memory[phys_addr];
}

static inline uint32_t get_phys_unaligned_mem(struct CPUState *env, uint32_t phys_addr)
{
    uint8_t *ptr8;
    uint32_t *ptr32;

    ptr8 = (uint8_t*)env->memory;
    ptr8 += phys_addr;
    ptr32 = (uint32_t*)ptr8;

    return *ptr32;
}

#if 0
uint32_t get_virt_mem(struct CPUState *env, uint32_t virt_addr)
{
    /* steps:
     * 1. calulate physical address
     * 2. use get_phys_mem()
     */
    uint32_t phys_addr = phys_to_virt(env, virt_addr);
    return get_phys_mem(env, phys_addr);
}
#endif

uint32_t get_mem(struct CPUState *env, uint32_t addr)
{
#if 0
    if (env->mmu_enable)
        return get_virt_mem(env, addr);
    else
        return get_phys_mem(env, addr);
#endif

    if ((addr % 4) == 0) {  /* aligned */
        return get_phys_mem(env, addr/4);
    } else {
        return get_phys_unaligned_mem(env, addr);
    }
}

uint32_t get_pc_mem(struct CPUState *env, uint32_t pc)
{
    return get_phys_mem(env, pc);
}

uint8_t get_mem_byte(struct CPUState *env, uint32_t addr)
{
    return get_mem(env, addr) & 0x0ff;
}

void set_mem(struct CPUState *env, uint32_t addr, uint32_t val)
{
    if (addr >= MEM_SIZE) {
        derror("write memory @ 0x%x failed\n", addr);
        return;
    }
    env->memory[addr/4] = val;  /* doremi 未驗證 */
}

void set_mem_byte(struct CPUState *env, uint32_t addr, uint8_t val)
{
    uint32_t tmp = get_mem(env, addr);
    set_mem(env, addr, (tmp & 0xffffff00) | (val & 0x0ff));
}
