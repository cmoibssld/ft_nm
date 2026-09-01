#ifndef SECTION_HEADER_TABLE_H
# define SECTION_HEADER_TABLE_H

# include <stddef.h> // for size_t

# include "identification.h" // for t_spec struct

typedef enum {
  TABLE_INCOMPLETE,
  SIZE_ERROR,
  SYMBOL_UNCOMPLETE,
  CORRECT,
} t_section_table_status;

t_section_table_status  read_table(const char * restrict loaded_file, const size_t loaded_size, const t_spec * specs);

#endif
