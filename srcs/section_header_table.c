#include <elf.h>
#include <inttypes.h> // For the PRIx64 macro...
#include <stddef.h>
#include <stdint.h>

#include "endian.h"
#include "identification.h"
#include "section_header_info.h"
#include "symbols_sorted_array.h"
#include "section_header_table.h"
#include "output_formatting.h"


 const char *  get_string_table(const char* loaded_file, const void * section_header, const t_spec *specs, const t_section_table_data *info)
{
  const void * sh_table;
  uint32_t     sh_link;

  sh_table = loaded_file + info->address;
  if (specs->arch == X32_BIT)
    sh_link = specs->e == LITTLE ? ((Elf32_Shdr *)section_header)->sh_link : endian_swap32(((Elf32_Shdr *)section_header)->sh_link);
  else
    sh_link = specs->e == LITTLE ? ((Elf64_Shdr *)section_header)->sh_link : endian_swap32(((Elf64_Shdr *)section_header)->sh_link);
  if (sh_link >= info->total_entry)
    return (NULL);
  if (specs->arch == X32_BIT)
  {
    if (specs->e == LITTLE)
    {
      return (loaded_file + (((Elf32_Shdr *)(sh_table))[sh_link]).sh_offset);
    }
    else
      return (loaded_file + endian_swap32((((Elf32_Shdr *)(sh_table))[sh_link]).sh_offset)); // swap both offset: sh_link and sh_offset
  }
  else
  {
    if (specs->e == LITTLE)
    {
      return (loaded_file + (((Elf64_Shdr *)(sh_table))[sh_link]).sh_offset);
    }
    else
      return (loaded_file + endian_swap64((((Elf64_Shdr *)(sh_table))[sh_link]).sh_offset));
    }
}  

bool  overlapping_string_name(const char *s1, const char *s2)
// unused: nm doens't verify this. Might use it later.
{
  size_t i;

  i = 0;
  if (s1 < s2)
  {
    while (s1[i] != '\0')
    {
      if (&s1[i] == s2)
        return (true);
      ++i;
    }
    return (&s1[i] == s2);
  }
  else
  {
    while (s2[i] != '\0')
    {
      if (&s2[i] == s1)
        return (true);
      ++i;
    }
    return (&s1[i] == s2);
  }
  // s1 < s2 is not strictly portable C -> undefined behavior. So must do both strings iterate.
  // not really sure about that though... It seems obvious to me that address as hexa could be compared...
  // In deed if s1 and s2 are the same object, there is no problem about it, which is the case here
}


t_section_table_status  read_table(const char *restrict loaded_file, const size_t loaded_size, const t_spec *specs, const t_section_table_data *info, const t_options *opt)
// Note: t_section_table_data information are on litlle endiant coded. So no biggy to compare them with anything not from the file !
{
  s_symbol  *symbols_array;
  size_t    total_symbols;

  symbols_array = create_array(loaded_file, loaded_size, specs, info, &total_symbols);
  if (symbols_array == NULL)
    return(MEMORY_ALLOC_ERROR);
  sort_array(symbols_array, total_symbols, specs, opt);
  print_array(symbols_array, total_symbols, specs, opt);
  // print_array_important_stuff(symbols_array, specs, total_symbols);
  free(symbols_array);
  return (CORRECT);
}
