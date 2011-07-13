#ifndef _ARM_INST_H
#define _ARM_INST_H

#include <stdint.h>
#include "env.h"

#ifdef _DEBUG_
#define derror printf
#else
#define derror(str, ...)
#endif

enum {
    CLASS_DATA_PROCESSING,
    CLASS_LOAD_STORE,
};

enum {
    OP_AND,
    OP_EOR,
    OP_SUB,
    OP_RSB,

    OP_ADD,
    OP_ADC,
    OP_SBC,
    OP_RSC,

    OP_TST,
    OP_TEQ,
    OP_CMP,
    OP_CMN,

    OP_ORR,
    OP_MOV,
    OP_BIC,
    OP_MVN,
};

#define ALU_OP_MASK     0x01e00000
#define ALU_OP_SHIFT    21

#define RD_MASK         0x0000f000
#define RD_SHIFT        12
#define RN_MASK         0x000f0000
#define RN_SHIFT        16
#define RM_MASK         0x0000000f
#define RM_SHIFT        0

#define IMM12_MASK      0x00000fff
#define IMM5_MASK       0x00000f80
#define IMM5_SHIFT      7
#define TYPE_MASK       0x00000060
#define TYPE_SHIFT      5

#define SFLAG_MASK      0x00100000
#define SFLAG_SHIFT     20

#define BIT0    0x00000001
#define BIT1    0x00000002
#define BIT2    0x00000004
#define BIT3    0x00000008
#define BIT4    0x00000010
#define BIT5    0x00000020
#define BIT6    0x00000040
#define BIT7    0x00000080
#define BIT8    0x00000100
#define BIT9    0x00000200
#define BIT10   0x00000400
#define BIT11   0x00000800
#define BIT12   0x00001000
#define BIT13   0x00002000
#define BIT14   0x00004000
#define BIT15   0x00008000
#define BIT16   0x00010000
#define BIT17   0x00020000
#define BIT18   0x00040000
#define BIT19   0x00080000
#define BIT20   0x00100000
#define BIT21   0x00200000
#define BIT22   0x00400000
#define BIT23   0x00800000
#define BIT24   0x01000000
#define BIT25   0x02000000
#define BIT26   0x04000000
#define BIT27   0x08000000
#define BIT28   0x10000000
#define BIT29   0x20000000
#define BIT30   0x40000000
#define BIT31   0x80000000

void print_preamble(struct CPUState *env, uint32_t inst);

static inline int getbit(uint32_t val, uint32_t bit)
{
    return (val & bit) > 0;
}

#endif

