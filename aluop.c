#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "inst.h"
#include "aluop.h"
#include "reg.h"
#include "env.h"
#include "load_store.h"
#include "cond.h"
#include "utils.h"

int no_op(struct CPUState *env, uint32_t inst)
{
    return 0;
}

int mov_reg(struct CPUState *env, uint32_t inst)
{
    uint32_t rd = getrd(inst);
    uint32_t rm = getrm(inst);
    uint32_t imm5 = getimm5(inst);
    uint32_t type = gettype(inst);

    print_preamble(env, inst);

    /* 在這裡判斷五種: mov, lsl, lsr, asr, rrx, ror */

    switch (type) {
    case 0: /* mov or lsl */
        if (imm5) { /* lsl */
            printf("Unfinished lsl\n");
            exit(1);
        } else {    /* mov */
            print_inst("mov", inst);
        }
        break;
    default:
        printf("Unfinished mov shift\n");
        exit(1);
    }

    printf("\t%s, %s\n", reg_name(rd), reg_name(rm));

/* implement */

    switch (type) {
    case 0: /* mov or lsl */
        if (imm5) { /* lsl */
            printf("Unfinished lsl\n");
            exit(1);
        } else {    /* mov */
            if (!check_cond(env, inst))
                return 0;
            set_reg(env, rd, get_reg(env, rm));
            if (sflag(inst) && rd != REG_PC) {
                env->cpsr.N = getbit(inst, BIT31);
                env->cpsr.Z = (rm == 0);

            }
        }
        break;
    default:
        printf("Unfinished mov shift\n");
        exit(1);
    }

    return 0;
}

uint32_t add_with_carry(
    uint32_t x, uint32_t y, uint32_t carry_in,
    struct CPUState *env, int update)
{
    uint32_t usum = x + y + carry_in;
    int32_t ssum = (int32_t)x + (int32_t)y + carry_in;
    uint32_t result = usum & 0x7fffffff;
    uint32_t carry_out = ((uint32_t)result == usum) ? 0 : 1;
    uint32_t overflow = ((int32_t)result == ssum) ? 0 : 1;

    if (update) {
        env->cpsr.N = getbit(result, BIT31);
        env->cpsr.Z = (result == 0 ? 1 : 0);
        env->cpsr.C = carry_out;
        env->cpsr.V = overflow;
    }

    return result;
}

int add_reg(struct CPUState *env, uint32_t inst)
{
    uint32_t rd = getrd(inst);
    uint32_t rm = getrm(inst);
    uint32_t rn = getrn(inst);
    uint32_t type = gettype(inst);
    uint32_t imm5 = getimm5(inst);
    uint32_t sh, shifted, result;
    const char *str;

    print_preamble(env, inst);
    print_inst("add", inst);
    printf("\t%s, %s, %s", reg_name(rd), reg_name(rn), reg_name(rm));
    str = shift_type(type, imm5);
    if (str[0] != '\0')
        printf(", %s #%d", str, imm5);
    printf("\n");

    if (!check_cond(env, inst))
        return 0;
    sh = decode_imm_shift(type, imm5);
    shifted = shift(env, rm, type, sh);
    result = add_with_carry(rn, shifted, 0, env, (sflag(inst) && rd != REG_PC));
    set_reg(env, rd, result);

    return 0;
}

int mov_imm(struct CPUState *env, uint32_t inst)
{
    uint32_t rd = getrd(inst);
    uint32_t imm12 = getimm12(inst);
    uint32_t unrotated_value = (imm12 & 0xff);  /* imm12<7:0> */
    uint32_t rot = 2 * ((imm12 & 0xf00) >> 8);
    uint32_t value = shift(env, unrotated_value, TYPE_ROR, rot);

    print_preamble(env, inst);
    print_inst("mov", inst);
    if (value > 32)
        printf("\t%s, #%d\t; 0x%x\n", reg_name(rd), value, value);
    else
        printf("\t%s, #%d\n", reg_name(rd), value);

    set_reg(env, rd, value);
    if (sflag(inst) && rd != REG_PC) {
        env->cpsr.N = getbit(inst, BIT31);
        env->cpsr.Z = (rd == 0);
/* FIXME:
 * I'm not sure whether carry bit should be set or not
 */
//        env->cpsr.C = doremi
    }

    return 0;
}

int (*alu_reg_op[16])(struct CPUState *, uint32_t) = {
    no_op, no_op, no_op, no_op,
    add_reg, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op,
    no_op, mov_reg, no_op, no_op
};

int (*alu_imm_op[16])(struct CPUState *, uint32_t) = {
    no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op,
    no_op, mov_imm, no_op, no_op
};
