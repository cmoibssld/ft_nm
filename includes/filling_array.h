#ifndef FILLING_ARRAY
# define FILLING_ARRAY

#include "section_header_info.h"
# include "symbols_sorted_array.h"
#include <uchar.h>

typedef enum {
  OUT_OF_BOUND,
  UNFINISHED_STRING,
  GENERAL_ERROR,
  FILLING_OK,
} FILLING_STATUS;

FILLING_STATUS  fill_array_per_section(s_symbol *symbol_array, const char *loaded_file, const t_spec *specs, const t_section_table_data *info);
FILLING_STATUS  fill_array_per_symbols(s_symbol *symbol_array, const char *loaded_file, const void *section_header, const void *section_table, const char *strtab, const t_spec *specs, size_t *s_array_idx);

const void      *get_symbol_ptr(const char *loaded_file, const void *section_header, const size_t idx, const t_spec *specs);
const char      *get_symbol_name(const void *symbol_header, const char *strtab, const t_spec *specs);
const void      *get_section_ptr(const void *sym, const void *section_table, const t_spec *specs);
const char      *get_section_name( const void *section_header, const char *strtab, const t_spec *specs);

#endif
