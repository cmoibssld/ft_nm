#include "symbols_sorted_array.h"
#include "endian.h"
#include "identification.h"
#include "section_header_info.h"
#include <elf.h>
#include <stddef.h>
#include <stdint.h>

// On the method. Allow a table of pointer onto Symbols headers. Then sort it, so it's like nm output.
// Since file in in memory, looping twice (one to know the total size of the array, the second one to fill it) is not important. Twice, yes but one, knowned-before allocation.
// Better than malloc and realloc. 

ssize_t  looping_on_sections(const char *loaded_file, const size_t loaded_size, const t_spec *specs, const t_section_table_data *info)
{
  size_t      total_symbols;
  uint16_t    i;
  size_t      symbols_header_size;
  uint32_t    sh_type;
  const void  *section_header;
  
  total_symbols = 0;
  i = 0;
  symbols_header_size = specs->arch == X32_BIT ? sizeof(Elf32_Sym) : sizeof(Elf64_Sym);

  while (i < info->total_entry)
  {
    if (loaded_size < info->address + i * symbols_header_size)
      return (-1);
    // Check if header is symbols aka SHT_SYMTAB
    section_header = specs->arch == X32_BIT ? (void *)&((Elf32_Shdr *)(loaded_file + info->address))[i] : (void *)&((Elf64_Shdr *)(loaded_file + info->address))[i];
    sh_type = specs->arch == X32_BIT ? ((Elf32_Shdr *)section_header)->sh_type : ((Elf64_Shdr *)section_header)->sh_type;
    sh_type = specs->e == LITTLE ? sh_type : endian_swap32(sh_type);
    if (sh_type == SHT_SYMTAB)
      total_symbols += looping_on_symbols(section_header, loaded_size, specs, symbols_header_size);
    // need a good way to add + verify if looping is -1...
    ++i;
  }  
  return (total_symbols);
}

ssize_t  looping_on_symbols(const void *section_header, const size_t loaded_size, const t_spec *specs, const size_t symbol_size)
{
  uint32_t  section_size;
  uint16_t  i;
  bool      out_of_bound;

  section_size = specs->arch == X32_BIT ? ((Elf32_Shdr *)section_header)->sh_size : ((Elf64_Shdr *)section_header)->sh_size;
  section_size = specs->e == LITTLE ? section_size: endian_swap32(section_size);
  i = 0;
  while (i * symbol_size < section_size)
  {
    out_of_bound = true;
    if (specs->arch == X32_BIT)
    {
      out_of_bound = specs->e == LITTLE ? ((Elf32_Shdr *)section_header)->sh_offset + i * symbol_size > loaded_size : endian_swap32(((Elf32_Shdr *)section_header)->sh_offset) + i * symbol_size > loaded_size;
    }
    else if (specs->arch == X64_BIT)
    {
      out_of_bound = specs->e == LITTLE ? ((Elf64_Shdr *)section_header)->sh_offset + i * symbol_size > loaded_size : endian_swap64(((Elf64_Shdr *)section_header)->sh_offset) + i * symbol_size > loaded_size;
    }
    if (out_of_bound == true)
      return (-1);
    ++i;
  }
  return (i - 1); // first of the section is always a null symbol
}
