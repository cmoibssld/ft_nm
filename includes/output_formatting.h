#ifndef OUTPUT_FORMATTING
# define OUTPUT_FORMATTING

# include <stdbool.h>

#include "main.h"
# include "section_header_table.h" // old 
t_section_table_status  print_function_symbol(const uint8_t binding, const bool little_endian, const bool x32, const void *symbol_header, const char *string_table);
t_section_table_status  print_undefined_symbol(const bool little_endian, const bool x32, const void *symbol_header, const char *string_table);

# include "symbols_sorted_array.h"

int  print_array(const s_symbol *symbols, const size_t total_symbols, const t_spec *specs, const t_options *opt);

#endif
