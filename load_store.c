#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "inst.h"
#include "aluop.h"
#include "mem.h"
#include "reg.h"
#include "env.h"
#include "utils.h"

uint32_t decode_imm_shift(uint32_t type, uint32_t imm5)
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
        derror("undefined shift type, %s\n", __FUNCTION__);
        exit(1);
    }
}

const char *shift_type(uint32_t type, uint32_t imm5)
{
    static const char *t[] = {
        "lsl", "lsr", "asr", "ror", "rrx"
    };

    if (type == 0 && imm5 == 0)
        return "";
    if (type == 3) {
        if (imm5 == 0)
            return t[4];
        else
            return t[3];
    }

    return t[type];
}

uint32_t shift(struct CPUState *env, uint32_t val, uint32_t type, uint32_t imm5)
{
    uint64_t ff;
    uint32_t sh;

    sh = imm5;

    if (sh == 0)
        return val;

    switch (type) {
    case 0: /* lsl */
        return val << sh;
    case 1: /* lsr */
        return val >> sh;
    case 2: /* asr */
        if (getbit(val, BIT31) == 1) {  /* negative */
            ff = 0xffffffff00000000LL;
            return (ff | val) >> sh;
        } else
            return val >> sh;
    case 3:
        if (imm5) { /* ror */
            uint64_t tmpb;
            ff = val;
            ff <<= 32;
            ff >>= sh;
            val >>= sh;
            tmpb = ff & 0xffffffff;
            return val | tmpb;
        } else {    /* rrx */
            return (val >> 1) | (env->cpsr.C << 31);
        }
    default:        derror("undefined shift instruction\n");
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
    uint32_t P, U, B, W, L, rn, rt;
    uint32_t imm12;
    uint32_t offset_addr, address, data;

    P = getbit(inst, BIT24);
    U = getbit(inst, BIT23);
    B = getbit(inst, BIT22);
    W = getbit(inst, BIT21);
    L = getbit(inst, BIT20);
    rn = getrn(inst);
    rt = getrd(inst);
    imm12 = getimm12(inst);

    if (!check_cond(env, inst))
        return 0;
    /* 未驗證 */
    if (rn == REG_PC)
        offset_addr = env->pc+4;

    if (U)
        offset_addr += get_reg(env, rn) + imm12;
    else
        offset_addr += get_reg(env, rn) - imm12;

    if (P)
        address = offset_addr;
    else
        address = get_reg(env, rn);

    if (L)
        if (B)
            data = get_mem_byte(env, address);
        else
            data = get_mem(env, address);
    else
        if (B)
            set_mem_byte(env, address, get_reg(env, rt));
        else
            set_mem(env, address, get_reg(env, rt));

    if (W)
        set_reg(env, rn, offset_addr);
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

    if (!check_cond(env, inst))
        return 0;
    offset_addr = shift(env, get_reg(env, rm), type, imm5);

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

