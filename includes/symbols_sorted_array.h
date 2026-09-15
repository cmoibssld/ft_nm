#ifndef SYMBOLS_SORTED_ARRAY
# define SYMBOLS_SORTED_ARRAY

# include <elf.h>
# include <inttypes.h>
# include <stddef.h>

# include "main.h"
# include "section_header_info.h"

ssize_t  looping_on_sections(const char *loaded_file, const size_t loaded_size, const t_spec *specs, const t_section_table_data *info);
ssize_t  looping_on_symbols(const void *section_header, const size_t loaded_size, const t_spec *specs, const size_t symbol_size);

const void    **create_array(const char *loaded_file, const size_t loaded_size, const t_spec *specs, const t_section_table_data *info, size_t *total_symbols);
void    fill_array(const void **symbol_array, const char *loaded_file, const t_spec *specs, const t_section_table_data *info);
void    sort_array(const void **symbols_array, const t_spec *specs, const size_t total_symbols, const t_options *opt);

int    sym_compare(const void *sym_1, const void *sym_2);
int    rev_sym_compare(const void *sym_1, const void *sym_2);

// debugging purpose
void    print_array_important_stuff(const void **symbols_array, const t_spec *specs, const size_t total_symbols);

#endif

// p ((Elf64_Sym *)symbols_array)[
