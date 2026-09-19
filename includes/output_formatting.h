#ifndef OUTPUT_FORMATTING
# define OUTPUT_FORMATTING

#include <elf.h>
# include <stdbool.h>

# include "main.h"
# include "symbols_sorted_array.h"

// char  get_sym_flags(const char bind, const char type, const uint16_t sh_shndx, const char *name);
int  print_x32(const Elf32_Sym *sym, const Elf32_Shdr *section, const char *name, const t_elf_endian e, const t_options *opt);
int  print_x64(const Elf64_Sym *sym, const Elf64_Shdr *section, const char *name, const t_elf_endian e, const t_options *opt);
int  print_array(const s_symbol *symbols, const size_t total_symbols, const t_spec *specs, const t_options *opt);

#endif
