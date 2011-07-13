#ifndef _ENV_H
#define _ENV_H

#define REG_NUM 17  /* r0-r15, cpsr */
#define MEM_SIZE 4096

struct CPUState {
    uint32_t regs[REG_NUM];
    uint32_t memory[MEM_SIZE];
    uint32_t pc;
};

#endif
