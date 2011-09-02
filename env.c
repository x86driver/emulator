#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include "env.h"
#include "reg.h"

#define MEMORY_SIZE (65536l*65536l)

void init_cpu_state(struct CPUState *env)
{
    env->memory = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (env->memory == MAP_FAILED)
        perror("mmap");
    set_pc(env, 4); /* set_pc() will plus 4 automatically */
}

void clean_cpu_state(struct CPUState *env)
{
    munmap(env->memory, MEMORY_SIZE);
}
