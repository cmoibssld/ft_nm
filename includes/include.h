#ifndef FT_NM_H
#define FT_NM_H

#include <elf.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef enum {
  INVALID_ENDIAN,
  LITTLE,
  BIG,
} t_elf_endian;

typedef enum {
  INVALID_ARCH,
  X64_BIT,
  X32_BIT,
} t_elf_arch;

typedef struct elf_file_key_spec {
  t_elf_endian  e;
  t_elf_arch    arch;
  uint16_t      type;
} t_spec;

int   ft_error(char *str, int error_code);

int   ft_strlen(char *str);

int   putstr(char *str, int fd);

void  check_elf(unsigned char *addr, int fd, t_spec *spec, off_t file_size);

void  core_engine(char *addr, int fd, off_t size);

#endif