#include <stdio.h>

#if 1
void htoa(char *buf, unsigned int num)
{
    int i;
    char ret;
    char tmp[8];

    *buf++ = '0';
    *buf++ = 'x';
    for (i = 0; i < 8; ++i) {
        ret = num % 16;
        if (ret <= 9)
            ret += '0';
        else
            ret += 'W';
        tmp[i] = ret;
        num /= 16;
    }
    for (i = 7; i >= 0; --i) {
        *buf++ = tmp[i];
    }
    *buf = '\0';
}

#endif

void __attribute__((noinline)) mywrite(char *buf, size_t len)
{
#if 1
    asm volatile("mov r0, #1\n\t"
                 "mov r1, %0\n\t"
                 "mov r2, %1\n\t"
                 "mov r7, #4\n\t"
                 "svc #0\n\t"
                 ::"r"(buf), "r"(len):"r7");
#endif
#if 0
    asm volatile("mov r2, r1\n\t"
                 "mov r1, r0\n\t"
                 "mov r0, #1\n\t"
                 "mov r7, #4\n\t"
                 "svc #0\n\t":::"r7");
#endif
}

static inline void myexit(int status)
{
    asm volatile("mov r0, %0\n\t"
                 "mov r7, #1\n\t"
                 "svc #0\n\t"
                 ::"r"(status):"r7");
}

int main()
{

    char buf[16] = "";
    int a = 3;
    int b = 7;
    int c = a + b;

    c = 0xffffeeee;
    htoa(buf, c);

//    char buf[] = "Hello world\n";
    buf[10] = '\n';
    buf[11] = '\0';
    mywrite(buf, 12);

    myexit(0);
}
