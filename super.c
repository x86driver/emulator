#include <stdio.h>
#include "super.h"
#include "reg.h"

int svc_exit(struct CPUState *env)
{
    printf("\nEnd simulation @ 0x%x\n\n", get_reg(env, REG_PC));
    return 1;
}

int svc_write(struct CPUState *env)
{
    /* r0: fd (ignore)
     * r1: buffer
     * r2: length
     */

    uint32_t offset = get_reg(env, REG_R1);
    char *ptr = (char*)env->memory + offset;
    int len = get_reg(env, REG_R2);

    while (len--) {
        putc(*ptr++, stdout);
    }

    return 0;
}

int svc(struct CPUState *env, uint32_t inst)
{
    int ret = 0;
    int syscall = get_reg(env, REG_R7);
    inst = inst;

    switch (syscall) {
    case 1:
        ret = svc_exit(env);
        break;
    case 4:
        svc_write(env);
        break;
    }

    return ret;
}
