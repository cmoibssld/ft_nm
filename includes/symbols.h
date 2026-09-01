#ifndef SYMBOLS_H
# define SYMBOLS_H

# include "identifications.h" // t_specs structure -> what are we talking about

// Symbol type
enum {
  OBSOLETE,
  BSS_DATA_SECTION,
  COMMON,
  INITIALIZED_DATA_SECTION,
} elf_symbols;

bool  find_symbols(const char * restrict loaded_file, const size_t loaded_size, const t_specs * specs);

#endif
