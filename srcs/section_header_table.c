#include <elf.h>
#include <inttypes.h> // For the PRIx64 macro...
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "endian.h"
#include "identification.h"
#include "main.h"
#include "output_formatting.h"
#include "section_header_info.h"
#include "section_header_table.h"
#include "symbols_sorted_array.h"

// Elf32_Word and Elf64_Word is the same: uint16_t. GNE !


// From what I have understand on the Section header table.
// Indeed the section header table is an ELF header where are store as a table all the headers on the sections. They contain information on the section such as names, sizes, locations, etc.,.
// The first information store, sh_name, aka the name of the function does not point to a string. It is not a name. It is an offset of a string in ... the section name string table. This section name string table ahas it's index define inside the ELF header: e_shstrndx.
// The location of the section in the ELF file image is given bt the section header variable: sh_offset.
// Name are interesting but the first thing to check for each section in the table is the sh_type. It will tell us wether a section is about.. A symbol : SHT_SYMTAB ! 

 const char *  get_string_table(const char* loaded_file, const void * section_header, const t_spec *specs, const t_section_table_data *info)
{
  const void * sh_table;

  sh_table = loaded_file + info->address;
  if (specs->arch == X32_BIT)
  {
    if (specs->e == LITTLE)
    {
      return (loaded_file + (((Elf32_Shdr *)(sh_table))[((Elf32_Shdr *)(section_header))->sh_link]).sh_offset);
    }
    else
      return (loaded_file + endian_swap32((((Elf32_Shdr *)(sh_table))[endian_swap32(((Elf32_Shdr *)(section_header))->sh_link)]).sh_offset)); // swap both offset: sh_link and sh_offset
  }
  else
  {
    if (specs->e == LITTLE)
    {
      return (loaded_file + (((Elf64_Shdr *)(sh_table))[((Elf64_Shdr *)(section_header))->sh_link]).sh_offset);
    }
    else
      return (loaded_file + endian_swap64((((Elf64_Shdr *)(sh_table))[endian_swap64(((Elf64_Shdr *)(section_header))->sh_link)]).sh_offset));
    }
}

t_section_table_status  read_table(const char *restrict loaded_file, const size_t loaded_size, const t_spec *specs, const t_section_table_data *info, const t_options *opt)
// Note: t_section_table_data information are on litlle endiant coded. So no biggy to compare them with anything not from the file !
{
  s_symbol  *symbols_array;
  size_t    total_symbols;

  symbols_array = create_array(loaded_file, loaded_size, specs, info, &total_symbols);
  if (symbols_array == NULL)
    return(MEMORY_ALLOC_ERROR);
  sort_array(symbols_array, total_symbols, opt);
  print_array(symbols_array, total_symbols, specs, opt);
  // print_array_important_stuff(symbols_array, specs, total_symbols);
  free(symbols_array);
  return (CORRECT);
}
