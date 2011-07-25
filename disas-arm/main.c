#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void disas(FILE *out, void *code, unsigned long size);

int main()
{
    void *code = malloc(4096);
    int fd = open("a.bin", O_RDONLY);
    unsigned long size = read(fd, code, 4096);
    close(fd);

    disas(stdout, code, size);

    return 0;
}

