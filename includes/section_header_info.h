#ifndef SECTION_HEADER_INFO_H
# define SECTION_HEADER_INFO_H

# include <stdatomic.h>
# include <stdint.h>

# include "identification.h" // t_specs structure -> what are we talking about

// Symbol type
typedef enum {
  OBSOLETE,
  BSS_DATA_SECTION,
  COMMON,
  INITIALIZED_DATA_SECTION,
} elf_symbols;

typedef struct {
  uint64_t  address;
  uint16_t  entry_size;
  uint16_t  total_entry;
  uint16_t  string_index;
} t_section_table_data;

uint64_t get_section_header_offset(const char * restrict loaded_file, const size_t loaded_size, const t_spec * specs); 
uint16_t  get_section_header_entry_size(const char * restrict loaded_file, const size_t loaded_size, const t_spec * specs);
uint16_t  get_section_header_total_entry(const char * restrict loaded_file, const size_t loaded_size, const t_spec * specs);
uint16_t  get_section_header_string_table_index(const char * restrict loaded_file, const size_t loaded_size, const t_spec * specs);
bool  find_header_table_info(const char * restrict loaded_file, const size_t loaded_size, const t_spec * specs, t_section_table_data * info);


// function for test purpose only
void  print_offset_res(const uint16_t table_offset, const uint16_t section_entry_size, const uint16_t total_entry, const uint16_t string_index);

#endif
