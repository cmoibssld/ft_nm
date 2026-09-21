#ifndef FT_NM_H
#define FT_NM_H

#include <elf.h>
#include <fcntl.h>
#include "../libft/includes/libft.h"
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

  void *addr;
  off_t size;
} t_spec;

typedef struct {
    void *sym;
    char *name;
    uint64_t value;
    unsigned char c;
} t_nm_sym;

typedef struct {
    bool a;
    bool g;
    bool u;
    bool r;
    bool p;
} t_nm_args;

int       ft_error(char *str, int error_code);

int       putstr(char *str, int fd);

int       check_elf(unsigned char *addr, int fd, t_spec *spec, off_t file_size);

int       core_engine_32(char *addr, off_t size, t_spec *spec, t_nm_args *nm_args, bool swap);

int       core_engine_64(char *addr, off_t size, t_spec *spec, t_nm_args *nm_args, bool swap);

int       compare_symbols(const void *a, const void *b);

int       rev_compare_symbols(const void *a, const void *b);

int       nm_error(char *filename, char *custom_error, char *flag);

int       nm_file_error(char *filename, char *custom_error, char *flag);

int       print_error(char *str);

uint16_t  swap16(uint16_t v);

uint32_t  swap32(uint32_t v);

uint64_t  swap64(uint64_t v);

#define SWAP16(val, swap) ((swap) ? swap16(val) : (val))
#define SWAP32(val, swap) ((swap) ? swap32(val) : (val))
#define SWAP64(val, swap) ((swap) ? swap64(val) : (val))

#endif