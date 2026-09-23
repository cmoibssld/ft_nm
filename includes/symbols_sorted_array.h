#ifndef SYMBOLS_SORTED_ARRAY
# define SYMBOLS_SORTED_ARRAY

# include <elf.h>
# include <inttypes.h>
# include <stdbool.h>
# include <stddef.h>
# include <sys/types.h>

# include "main.h"
# include "section_header_info.h"

typedef struct {
  const void  *section;
  const void  *sym;
  const char  *name;
} s_symbol;

ssize_t  looping_on_sections(const char *loaded_file, const size_t loaded_size, const t_spec *specs, const t_section_table_data *info);
ssize_t  looping_on_symbols(const void *section_header, const size_t loaded_size, const t_spec *specs, const size_t symbol_size);

s_symbol    *create_array(const char *loaded_file, const size_t loaded_size, const t_spec *specs, const t_section_table_data *info, size_t *total_symbols);
bool        check_strings_name(const s_symbol *symbol_array, const size_t total_symbols, const char *loaded_file, const size_t loaded_size);
void    sort_array(s_symbol *symbols_array, const size_t total_symbols, const t_spec *specs, const t_options *opt);

#endif
