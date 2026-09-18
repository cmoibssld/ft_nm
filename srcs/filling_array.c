#include "filling_array.h"
#include "endian.h"
#include "identification.h"
#include <elf.h>
#include <stddef.h>
#include <stdint.h>

FILLING_STATUS  fill_array_per_section(s_symbol *symbol_array, const char *loaded_file, const size_t loaded_size, const t_spec *specs, const t_section_table_data *info)
{
  size_t      section_idx;
  size_t      section_size;
  const void  *section_header;
  uint16_t    sh_type;

  section_idx = 0;
  section_size = specs->arch == X32_BIT ? sizeof(Elf32_Shdr) : sizeof(Elf64_Shdr);
  section_header = loaded_file + info->address;
  while (section_idx < info->total_entry)
  {
    // check if offset is out of loaded_size?
    if (info->address + section_size * (section_idx + 1) >= loaded_size)
      return (OUT_OF_BOUND);
    sh_type = specs->arch == X32_BIT ? ((Elf32_Shdr *)section_header)[section_idx].sh_type : ((Elf64_Shdr *)section_header)[section_idx].sh_type;
    sh_type = specs->e == LITTLE ? sh_type : endian_swap16(sh_type);
    if (sh_type == SHT_SYMTAB)
      if (fill_array_per_symbols(symbol_array, loaded_file, loaded_size, specs) != FILLING_OK)
          return (GENERAL_ERROR);
    ++section_idx;
    
  }
  return (FILLING_OK);
}

FILLING_STATUS  fill_array_per_symbols(s_symbol *symbol_array, const char *loaded_file, const size_t loaded_size, const t_spec *specs)
{
  return (FILLING_OK);
}


