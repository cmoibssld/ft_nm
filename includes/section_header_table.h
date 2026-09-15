#ifndef SECTION_HEADER_TABLE_H
# define SECTION_HEADER_TABLE_H

# include <stddef.h> // for size_t
# include <stdint.h>

# include "identification.h" // for t_spec struct
# include "section_header_info.h"

typedef enum {
  TABLE_INCOMPLETE,
  SIZE_ERROR,
  SYMBOL_UNCOMPLETE,
  CORRECT,
  MEMORY_ALLOC_ERROR,
} t_section_table_status;

t_section_table_status  read_table(const char * restrict loaded_file, const size_t loaded_size, const t_spec * specs, const t_section_table_data *info, const t_options *opt);

t_section_table_status  read_as_32bit(const char *loaded_file, const size_t loaded_size, const bool little_endian, const t_section_table_data *info);
t_section_table_status  read_as_64bit(const char *loaded_file, const size_t loaded_size, const bool little_endian, const t_section_table_data *info);

t_section_table_status  symbol_table_id(const uint16_t st_info, const bool litlle_endian, const bool x32, const void *symbol_header, const char *string_table);

#endif
