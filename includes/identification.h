#ifndef IDENTIFICATION_H
# define IDENTIFICATION_H

# include <stdbool.h> // for bool use
# include <stdint.h>
# include <stdlib.h> // for size_t, uintXX_t types

# include "main.h"

typedef enum {
  NOT_A_ELF,
  NOT_A_VALID_ARCH,
  NOT_A_VALID_ENDIAN,
  NOT_A_VALID_VERSION,
  NOT_A_VALID_OBJECT,
  VALID_EFL_FILE,
} t_elf_id;

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
  t_elf_id      status;
  t_elf_endian  e;
  t_elf_arch    arch;
  uint16_t      type;
} t_spec;

int  core_logic(const char * restrict file_name, const char * restrict loaded_file, size_t loaded_size, const t_options *opt);

bool  is_a_correct_elf_file(const char * restrict loaded_file, t_spec *specs);

t_elf_endian  endian_identification(const char * restrict loaded_file);
t_elf_arch    arch_control(const char * restrict loaded_file);

#endif
