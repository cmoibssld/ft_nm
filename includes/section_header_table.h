#ifndef SECTION_HEADER_TABLE_H
# define SECTION_HEADER_TABLE_H

# include <stddef.h> // for size_t

# include "identification.h" // for t_spec struct
# include "section_header_info.h"

typedef enum {
  TABLE_INCOMPLETE,
  SIZE_ERROR,
  SYMBOL_UNCOMPLETE,
  CORRECT,
} t_section_table_status;

t_section_table_status  read_table(const char * restrict loaded_file, const size_t loaded_size, const t_spec * specs, const t_section_table_data *info);

t_section_table_status  read_as_32bit(const char *loaded_file, const size_t loaded_size, const t_section_table_data *info);
t_section_table_status  read_as_64bit(const char *loaded_file, const size_t loaded_size, const t_section_table_data *info);

#endif
