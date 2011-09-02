#ifndef _ENV_H
#define _ENV_H

#include <stdint.h>

#define REG_NUM 16  /* r0-r15 */
#define MEM_SIZE 4096

enum {
    MODE_USR = 0,
    NUM_MODES
};

struct Reg_CPSR {
    uint32_t N, Z, C, V;
};

struct Reg_PSR {
    uint32_t spsr[NUM_MODES];
};

struct CPUState {
    uint32_t regs[REG_NUM];
    uint32_t *memory;
    uint32_t pc;
    int mode;
    struct Reg_CPSR cpsr;
    struct Reg_PSR psr;
};

void init_cpu_state(struct CPUState *env);
void clean_cpu_state(struct CPUState *env);

static inline int get_mode(struct CPUState *env)
{
    return env->mode;
}

#endif
