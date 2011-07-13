#ifndef _ENV_H
#define _ENV_H

#define REG_NUM 16  /* r0-r15 */
#define MEM_SIZE 4096

struct Reg_CPSR {
    uint32_t c;
};

struct CPUState {
    uint32_t regs[REG_NUM];
    uint32_t memory[MEM_SIZE];
    uint32_t pc;
    struct Reg_CPSR cpsr;
};

#endif
