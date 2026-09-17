#ifndef FT_NM_H
#define FT_NM_H

#include <elf.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
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
  char*         filename;
  t_elf_endian  e;
  t_elf_arch    arch;
  uint16_t      type;
} t_spec;

typedef struct {
    void *sym;
    char *name;
    uint64_t value;
} t_nm_sym;

int     ft_error(char *str, int error_code);

int     ft_strlen(char *str);

int     putstr(char *str, int fd);

void    check_elf(unsigned char *addr, int fd, t_spec *spec, off_t file_size);

void    core_engine_32(char *addr, int fd, off_t size, t_spec *spec, bool swap);

void    core_engine_64(char *addr, int fd, off_t size, t_spec *spec, bool swap);

int     compare_symbols(const void *a, const void *b);

uint16_t swap16(uint16_t v);

uint32_t swap32(uint32_t v);

uint64_t swap64(uint64_t v);

#define SWAP16(val, swap) ((swap) ? swap16(val) : (val))
#define SWAP32(val, swap) ((swap) ? swap32(val) : (val))
#define SWAP64(val, swap) ((swap) ? swap64(val) : (val))

#endif