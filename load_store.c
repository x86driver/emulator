#include <stdio.h>
#include <stdint.h>
#include "inst.h"
#include "aluop.h"
#include "mem.h"
#include "reg.h"
#include "env.h"

int op_ldr(struct CPUState *env, uint32_t inst)
{
    uint32_t P, U, B, W, rn, rd;
    uint32_t imm12;
    uint32_t addr, val;

    P = getbit(inst, BIT24);
    U = getbit(inst, BIT23);
    B = getbit(inst, BIT22);
    W = getbit(inst, BIT21);
    rn = getrn(inst);
    rd = getrd(inst);
    imm12 = getimm12(inst);

    /* P = 0  ldr r0, [r1], #18    ; 會更改到 r1
     * P = 1  ldr r0, [r1, #18]    ; 根據 W
     * U = 0  減
     * U = 1  加
     * W = 1  ldr r0, [r1, #18]!
     */

    print_preamble(env, inst);
    if (getbit(inst, BIT25) == 0) { /* immediate */
        if (B)
            printf("ldrb");
        else
            printf("ldr");
        printf("\t%s, [%s", reg_name(rd), reg_name(rn));
        addr = get_mem(env, get_reg(env, rn));
        if (P) {
            if (B)
                val = get_mem_byte(env, addr);
            else
                val = get_mem(env, addr);
            if (imm12)
                if (U)
                    printf(", #%d]", imm12);
                else
                    printf(", #-%d]", imm12);
            else
                printf("]");
            if (W) {
                if (U)
                    set_reg(env, rn, addr + imm12);
                else
                    set_reg(env, rn, addr - imm12);
                printf("!");
            }
            if (imm12 > 32)
                printf("\t; 0x%x", imm12);
            printf("\n");
        } else {
            if (B)
                val = get_mem_byte(env, addr + imm12);
            else
                val = get_mem(env, addr + imm12);
            if (imm12)
                if (U)
                    printf("], #%d", imm12);
                else
                    printf("], #-%d", imm12);
            else
                printf("]");
            if (imm12 > 32)
                printf("\t; 0x%x", imm12);
            printf("\n");
            if (U)
                set_reg(env, rn, addr + imm12);
            else
                set_reg(env, rn, addr - imm12);
        }
    } else {    /* register */
    }
    return 0;
}

int op_str(struct CPUState *env, uint32_t inst)
{
    return 0;
}

