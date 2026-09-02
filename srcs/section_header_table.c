#include <elf.h>

#include "section_header_info.h"
#include "section_header_table.h"

// Elf32_Word and Elf64_Word is the same: uint16_t. GNE !

t_section_table_status  read_table(const char *restrict loaded_file, const size_t loaded_size, const t_spec *specs, const t_section_table_data *info)
// for now just read the table and how much there is in it
{
  char  *section_header_table;
  char  *e_shstrtab;

  // access section header table
  if (loaded_size < info->address + info->string_index)
    return (SIZE_ERROR);

  section_header_table = loaded_file + info->address;
  e_shstrtab = section_header_table + info->string_index;
    
  if (specs->arch == X32_BIT)
    return (read_as_32bit(loaded_file, loaded_size, info));
  if (specs->arch == X64_BIT)
    return (read_as_64bit(loaded_file,, loaded_size, info));
  // if (loaded_size < (size_t)info->total_entry * (size_t)info->entry_size + (size_t)info->address)
  //   return (SIZE_ERROR); // Problem: corrupt ELF file.
  // if (info->string_index == SHN_UNDEF)
  //   return (TABLE_INCOMPLETE);
  
  // return (CORRECT);
}
