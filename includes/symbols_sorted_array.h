#ifndef SYMBOLS_SORTED_ARRAY
# define SYMBOLS_SORTED_ARRAY

# include <elf.h>
# include <inttypes.h>
# include <stddef.h>

# include "section_header_info.h"

ssize_t  looping_on_sections(const char *loaded_file, const size_t loaded_size, const t_spec *specs, const t_section_table_data *info);

ssize_t  looping_on_symbols(const void *section_header, const size_t loaded_size, const t_spec *specs, const size_t symbol_size);

#endif
