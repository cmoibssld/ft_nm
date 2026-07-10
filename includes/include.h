#ifndef FT_NM_H
#define FT_NM_H

enum Architecture {
    ARCHITECTURE_64,
    ARCHITECTURE_32,
    ARCHITECTURE_INVALID,
};

#include <elf.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int ft_error(char *str, int error_code);
int ft_strlen(char *str);
int putstr(char *str, int fd);

#endif