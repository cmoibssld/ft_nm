#ifndef SYMBOLS_SORTED_ARRAY
# define SYMBOLS_SORTED_ARRAY

# include <elf.h>
# include <inttypes.h>
# include <stddef.h>

# include "main.h"
# include "section_header_info.h"

// Array question:
// an array of a struct by associating string-name in string table + pointer to symbol, can use sort easily because only comparing already created struct with correct point to string name (string_table + st_name). allocates once since allocate X struct in one array (so s_symbols * == the new array). Let's do that

typedef struct {
  const void  *sym;
  const char  *name;
} s_symbol;

ssize_t  looping_on_sections(const char *loaded_file, const size_t loaded_size, const t_spec *specs, const t_section_table_data *info);
ssize_t  looping_on_symbols(const void *section_header, const size_t loaded_size, const t_spec *specs, const size_t symbol_size);

s_symbol    *create_array(const char *loaded_file, const size_t loaded_size, const t_spec *specs, const t_section_table_data *info, size_t *total_symbols);
void    fill_array(s_symbol *symbol_array, const char *loaded_file, const t_spec *specs, const t_section_table_data *info);
void    sort_array(s_symbol *symbols_array, const size_t total_symbols, const t_options *opt);

// debugging purpose
void    print_array_important_stuff(s_symbol *symbols_array, const t_spec *specs, const size_t total_symbols);

#endif

// p ((Elf64_Sym *)symbols_array)[
