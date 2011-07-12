#define _BSD_SOURCE
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <endian.h>
#include <stdio.h>
#include <stdint.h>
#include "inst.h"

extern int (*alu_reg_op[16])(uint32_t);
extern int (*alu_imm_op[16])(uint32_t);

static inline int getbit(uint32_t val, uint32_t bit)
{
    return (val & bit) > 0;
}

int inst_class(uint32_t inst)
{
    /* FIXME:
     * Remember cond bit
     */
    uint32_t bit[32];

    bit[27] = getbit(inst, BIT27);
    bit[26] = getbit(inst, BIT26);

    if (bit[27] == 0 && bit[26] == 0) {
        return CLASS_DATA_PROCESSING;
    }

    return 0;
}

int dp_class(uint32_t inst)
{
    uint32_t bit[32];
    uint32_t aluop;

    bit[25] = getbit(inst, BIT25);
    bit[24] = getbit(inst, BIT24);
    bit[23] = getbit(inst, BIT23);
    bit[20] = getbit(inst, BIT20);
    bit[4] = getbit(inst, BIT4);

    if (bit[25] == 0) { /* register */
        if (bit[24] == 1 && bit[23] == 0 && bit[20] == 0) {
        } else if (bit[4] == 0) {
            aluop = (inst & ALU_OP_MASK) >> ALU_OP_SHIFT;
            alu_reg_op[aluop](inst);
        }
    } else {    /* immediate */
        if (bit[24] == 1 && bit[23] == 0 && bit[20] == 0) {
        } else {
            aluop = (inst & ALU_OP_MASK) >> ALU_OP_SHIFT;
            alu_imm_op[aluop](inst);
        }
    }

    return 0;
}

int main()
{
    int fd;
    uint32_t inst;

    fd = open("a.bin", O_RDONLY);

    while (read(fd, &inst, 4) > 0) {
        switch (inst_class(inst)) {
        case CLASS_DATA_PROCESSING:
            dp_class(inst);
            break;
        default:
            printf("undefined instruction %x\n", inst);
            break;
        }
    }

    close(fd);

    return 0;
}
