#include <stdio.h>
#include <stdint.h>

/* print_preamble 直接在外面呼叫 */
/* check_cond 也可以考慮在外面呼叫 */

void disas_mov_reg(uint32_t inst)
{
    uint32_t rd = getrd(inst);
    uint32_t rm = getrm(inst);
    uint32_t imm5 = getimm5(inst);
    uint32_t type = gettype(inst);

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
}

void add_reg(uint32_t inst)
{
    uint32_t rd = getrd(inst);
    uint32_t rm = getrm(inst);
    uint32_t rn = getrn(inst);
    uint32_t type = gettype(inst);
    uint32_t imm5 = getimm5(inst);
    uint32_t sh, shifted, result;
    const char *str;

    print_inst("add", inst);
    printf("\t%s, %s, %s", reg_name(rd), reg_name(rn), reg_name(rm));
    str = shift_type(type, imm5);
    if (str[0] != '\0')
        printf(", %s #%d", str, imm5);
    printf("\n");
}

void cmp_reg(uint32_t inst)
{
    uint32_t rm = getrm(inst);
    uint32_t rn = getrn(inst);
    uint32_t type = gettype(inst);
    uint32_t imm5 = getimm5(inst);
    uint32_t sh, shifted;

    print_inst_without_s("cmp", inst);
    printf("\t%s, %s\n", reg_name(rn), reg_name(rm));
}

void mov_imm(uint32_t inst)
{
    uint32_t rd = getrd(inst);
    uint32_t imm12 = getimm12(inst);
    uint32_t unrotated_value = (imm12 & 0xff);  /* imm12<7:0> */
    uint32_t rot = 2 * ((imm12 & 0xf00) >> 8);
    uint32_t value = shift(env, unrotated_value, TYPE_ROR, rot);

    print_inst("mov", inst);
    if (value > 32)
        printf("\t%s, #%d\t; 0x%x\n", reg_name(rd), value, value);
    else
        printf("\t%s, #%d\n", reg_name(rd), value);
}

void ldst_imm(struct CPUState *env, uint32_t inst)
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

    print_preamble(env, inst);

    print_inst_ldst((L ? "ldr" : "str"), inst);
    printf("\t%s, [%s", reg_name(rt), reg_name(rn));

    if (imm12 || rn == REG_PC) {
        if (P) {
            if (U)
                printf(", #%d]", imm12);
            else
                printf(", #-%d]", imm12);
        } else {
            if (U)
                printf("], #%d", imm12);
            else
                printf("], #-%d", imm12);
        }
    } else
        printf("]");

    if (W)
        printf("!");

    if (imm12 > 32)
        printf("\t; 0x%x", imm12);
    else if (rn == REG_PC) {
        uint32_t tmpaddr = env->pc+4;
        if (U)
            tmpaddr += imm12;
        else
            tmpaddr -= imm12;
        printf("\t; %x <.text+0x%x>", tmpaddr, tmpaddr);
    }
    printf("\n");
}

void ldst_reg(struct CPUState *env, uint32_t inst)
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

    print_inst_ldst((L ? "ldr" : "str"), inst);
    printf("\t%s, [%s", reg_name(rt), reg_name(rn));

    if (P) {
        if (U)
            printf(", %s", reg_name(rm));
        else
            printf(", -%s", reg_name(rm));
        if (imm5)
            printf(", %s #%d", shift_type(type, imm5), imm5);
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
}

void branch(struct CPUState *env, uint32_t inst)
{
    uint32_t imm24;

    print_preamble(env, inst);
    print_inst_without_s("b", inst);
    printf("\t");
    print_branch_pc(env, inst);
    printf("\n");
}
