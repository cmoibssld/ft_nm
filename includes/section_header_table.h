#ifndef SECTION_HEADER_TABLE_H
# define SECTION_HEADER_TABLE_H

# include <stdbool.h>
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

const char *  get_string_table(const char *loaded_file, const void *section_header, const t_spec *specs, const t_section_table_data *info);

bool          overlapping_string_name(const char *s1, const char *s2); 

#endif
