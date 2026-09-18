#ifndef FILLING_ARRAY
# define FILLING_ARRAY

#include "section_header_info.h"
# include "symbols_sorted_array.h"

typedef enum {
  OUT_OF_BOUND,
  UNFINISHED_STRING,
  GENERAL_ERROR,
  FILLING_OK,
} FILLING_STATUS;

FILLING_STATUS  fill_array_per_section(s_symbol *symbol_array, const char *loaded_file, const size_t loaded_size, const t_spec *specs, const t_section_table_data *info);
FILLING_STATUS  fill_array_per_symbols(s_symbol *symbol_array, const char *loaded_file, const size_t loaded_size, const t_spec *specs);

const void     *get_symbol_ptr(const char *loaded_file, const size_t loaded_size, const t_spec *specs);
const char      *get_symbol_name(const char *loaded_file, const size_t loaded_size, const t_spec *specs, const t_section_table_data *info);

#endif
