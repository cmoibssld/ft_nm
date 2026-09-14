#ifndef OUTPUT_FORMATTING
# define OUTPUT_FORMATTING

# include <stdbool.h>

# include "section_header_table.h"

t_section_table_status  print_function_symbol(const uint8_t binding, const bool little_endian, const bool x32, const void *symbol_header, const char *string_table);

#endif
