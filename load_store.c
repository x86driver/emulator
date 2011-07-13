#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "inst.h"
#include "aluop.h"
#include "mem.h"
#include "reg.h"
#include "env.h"

static uint32_t decode_imm_shift(uint32_t type, uint32_t imm5)
{
    switch (type) {
    case 0:
        return imm5;
    case 1:
    case 2:
        if (imm5 == 0)
            return 32;
        else
            return imm5;
    case 3:
        if (imm5 == 0)
            return 1;
        else
            return imm5;
    default:
        error("undefined shift type, %s\n", __FUNCTION__);
        exit(1);
    }
}

static char *shift_type(uint32_t type)
{
    static char *t[] = {
        "lsl", "lsr", "asr", "ror"
    };

    return t[type];
}

static uint32_t shift(uint32_t val, uint32_t type, uint32_t imm5)
{
    switch (type) {
    case 0:
        return val << decode_imm_shift(type, imm5);
    case 1:
        return val >> decode_imm_shift(type, imm5);
    default:
        error("undefined shift instruction\n");
        exit(1);
        break;
    }
}

/* P = 0  ldr r0, [r1], #18    ; 會更改到 r1
 * P = 1  ldr r0, [r1, #18]    ; 根據 W
 * U = 0  減
 * U = 1  加
 * W = 1  ldr r0, [r1, #18]!
 * L = 1  load
 * L = 0  store
 * Rm != r15
 */

int ldst_imm(struct CPUState *env, uint32_t inst)
{
    uint32_t P, U, B, W, L, rn, rd;
    uint32_t imm12;
    uint32_t addr, val;

    P = getbit(inst, BIT24);
    U = getbit(inst, BIT23);
    B = getbit(inst, BIT22);
    W = getbit(inst, BIT21);
    L = getbit(inst, BIT20);
    rn = getrn(inst);
    rd = getrd(inst);
    imm12 = getimm12(inst);

    print_preamble(env, inst);
    printf("%s", (L ? "ldr" : "str"));
    if (B)
        printf("b");
    printf("\t%s, [%s", reg_name(rd), reg_name(rn));
    if (rn == REG_PC)
        addr = get_mem(env, get_reg(env, rn) + 8);
    else
        addr = get_mem(env, get_reg(env, rn));
    if (P) {
        if (B) {
            if (L)
                val = get_mem_byte(env, addr);
            else
                set_mem_byte(env, addr, imm12);
        } else {
            if (L)
                val = get_mem(env, addr);
            else
                set_mem(env, addr, imm12);
        }
        if (imm12 || rn == REG_PC)
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
        if (rn == REG_PC)
            printf("\t; %x <.text+0x%x>", env->pc+4+imm12, env->pc+4+imm12);
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
    return 0;
}

int ldst_reg(struct CPUState *env, uint32_t inst)
{
    uint32_t P, U, B, W, L, rn, rt, rm;
    uint32_t imm5, type;
    uint32_t addr, offset_addr, data;

    P = getbit(inst, BIT24);
    U = getbit(inst, BIT23);
    B = getbit(inst, BIT22);
    W = getbit(inst, BIT21);
    L = getbit(inst, BIT20);
    rn = getrn(inst);
    rt = getrd(inst);
    rm = getrm(inst);
    imm5 = getimm5(inst);
    type = gettype(inst);

    print_preamble(env, inst);
    if (rm == REG_PC) {
        printf("undefined instruction\n");
        return -1;
    }

    printf("%s", (L ? "ldr" : "str"));

    if (B)
        printf("b");
    printf("\t%s, [%s", reg_name(rt), reg_name(rn));
    if (P) {
        if (U)
            printf(", %s", reg_name(rm));
        else
            printf(", -%s", reg_name(rm));
        if (imm5)
            printf(", %s #%d", shift_type(type), imm5);
        printf("]");
        if (W)
            printf("!");
    } else {
        if (U)
            printf("], %s", reg_name(rm));
        else
            printf("], -%s", reg_name(rm));
    }
    printf("\n");

    offset_addr = shift(get_reg(env, rm), type, imm5);    /* 要加 shift */

    if (U)
        offset_addr += get_mem(env, get_reg(env, rn) + offset_addr);
    else
        offset_addr -= get_mem(env, get_reg(env, rn) - offset_addr);

    if (P)
        addr = offset_addr;
    else
        addr = get_reg(env, rn);

    data = get_mem(env, get_reg(env, rt));

    if (B)
        set_mem_byte(env, addr, data);
    else
        set_mem(env, addr, data);

    if (W)
        set_reg(env, rn, offset_addr);

    return 0;
}

