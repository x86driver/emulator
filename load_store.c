#include <stdio.h>
#include <stdint.h>
#include "inst.h"
#include "aluop.h"

int op_ldr(uint32_t inst)
{
    uint32_t P, U, B, W, rn, rd;
    uint32_t imm12;
    uint32_t addr;

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

    print_preamble();
    if (getbit(inst, BIT25) == 0) { /* immediate */
        printf("ldr\t%s, [%s", rd, rn);
        if (P) {
            printf(", #%d]", getimm12(inst));
            if (W) {
                /* to be continued */
            }
        }
    } else {    /* register */
    }
    return 0;
}

int op_str(uint32_t inst)
{
    return 0;
}

