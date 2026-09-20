#include <elf.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "endian.h"
#include "filling_array.h"
#include "identification.h"
#include "libft.h"
#include "section_header_info.h"
#include "section_header_table.h"
#include "symbols_sorted_array.h"

FILLING_STATUS  fill_array_per_section(s_symbol *symbol_array, const char *loaded_file, const t_spec *specs, const t_section_table_data *info)
{
  size_t      section_idx;
  size_t      s_array_idx;
  const void  *section_header;
  uint16_t    sh_type;
  const char  *strtab;

  section_idx = 0;
  s_array_idx = 0;
  while (section_idx < info->total_entry)
  {
    section_header = specs->arch == X32_BIT ? (void *)&((Elf32_Shdr *)(loaded_file + info->address))[section_idx] : (void *)&((Elf64_Shdr *)(loaded_file + info->address))[section_idx];
    sh_type = specs->arch == X32_BIT ? ((Elf32_Shdr *)section_header)->sh_type : ((Elf64_Shdr *)section_header)->sh_type;
    sh_type = specs->e == LITTLE ? sh_type : endian_swap16(sh_type);
    if (sh_type == SHT_SYMTAB)
    {
      strtab = get_string_table(loaded_file, section_header, specs, info);
      if (strtab )
      if (fill_array_per_symbols(symbol_array, loaded_file, section_header, loaded_file + info->address, strtab, specs, &s_array_idx) != FILLING_OK)
          return (GENERAL_ERROR);
    }
    ++section_idx;
  }
  return (FILLING_OK);
}

FILLING_STATUS  fill_array_per_symbols(s_symbol *symbol_array, const char *loaded_file, const void *section_header, const void *section_table, const char *strtab, const t_spec *specs, size_t *s_array_idx)
{
  size_t    symbol_idx;
  size_t    symbol_size;
  uint16_t  section_size;

  symbol_idx = 1;
  symbol_size = specs->arch == X32_BIT ? sizeof(Elf32_Sym) : sizeof(Elf64_Sym);
  section_size = specs->arch == X32_BIT ? ((Elf32_Shdr *)section_header)->sh_size : ((Elf64_Shdr *)section_header)->sh_size;
  section_size = specs->e == LITTLE ? section_size : endian_swap16(section_size);

  while (symbol_idx * symbol_size < section_size)
  {
    symbol_array[*s_array_idx].sym = get_symbol_ptr(loaded_file, section_header, symbol_idx, specs);
    symbol_array[*s_array_idx].name = get_symbol_name(symbol_array[*s_array_idx].sym, strtab, specs);
    symbol_array[*s_array_idx].section = get_section_ptr(symbol_array[*s_array_idx].sym, section_table, specs);
    // maybe if symbol name is voided give it the section anme ?
    if (ft_strlen(symbol_array[*s_array_idx].name) == 0)
      symbol_array[*s_array_idx].name = get_section_name(symbol_array[*s_array_idx].section, strtab, specs); // works ! perfect
    ++(*s_array_idx);
    ++symbol_idx;
  }
  return (FILLING_OK);
}

const void  *get_symbol_ptr(const char *loaded_file, const void *section_header, const size_t idx, const t_spec *specs)
{
  if (specs->arch == X32_BIT)
  {
    if (specs->e == BIG)
      return (&((Elf32_Sym *)(loaded_file + endian_swap32(((Elf32_Shdr *)section_header)->sh_offset)))[idx]);
    else
      return (&((Elf32_Sym *)(loaded_file + ((Elf32_Shdr *)section_header)->sh_offset))[idx]);
  }
  else
  {
    if (specs->e == BIG)
      return (&((Elf64_Sym *)(loaded_file + endian_swap64(((Elf64_Shdr *)section_header)->sh_offset)))[idx]);
    else
      return (&((Elf64_Sym *)(loaded_file + ((Elf64_Shdr *)section_header)->sh_offset))[idx]);
  }
}

const char  *get_symbol_name(const void *symbol_header, const char *strtab, const t_spec *specs)
{
  if (specs->arch == X32_BIT)
  {
    if (specs->e == BIG)
      return (strtab + endian_swap32(((Elf32_Sym *)symbol_header)->st_name));
    else
      return (strtab + ((Elf32_Sym *)symbol_header)->st_name);
  }
  else
  {
    if (specs->e == BIG)
      return (strtab + endian_swap32(((Elf64_Sym *)symbol_header)->st_name));
    else
      return (strtab + ((Elf64_Sym *)symbol_header)->st_name);
  }
}

const void  *get_section_ptr(const void *sym, const void *section_table, const t_spec *specs)
{
  uint16_t  st_shndx;
  
  if (specs->arch == X32_BIT)
  {
      st_shndx = specs->e == LITTLE ? ((Elf32_Sym *)sym)->st_shndx : endian_swap16(((Elf32_Sym *)sym)->st_shndx);
      if (st_shndx == SHN_ABS) // || st_shndx == SHN_UNDEF || st_shndx == SHN_COMMON || st_shndx == SHN_LORESERVE)
        return (NULL);
      return (&((Elf32_Shdr *)section_table)[st_shndx]);
  }
  else
  {
      st_shndx = specs->e == LITTLE ? ((Elf64_Sym *)sym)->st_shndx : endian_swap16(((Elf64_Sym *)sym)->st_shndx);
      if (st_shndx == SHN_ABS) // || st_shndx == SHN_UNDEF || st_shndx == SHN_COMMON || st_shndx == SHN_LORESERVE)
        return (NULL);
      return (&((Elf64_Shdr *)section_table)[st_shndx]);
  }
}

const char  *get_section_name(const void *section_header, const char *strtab, const t_spec *specs)
{
  if (section_header == NULL)
    return (NULL);
  if (specs->arch == X32_BIT)
  {
    if (specs->e == BIG)
      return (strtab + endian_swap32(((Elf32_Shdr *)section_header)->sh_name));
    else
      return (strtab + ((Elf32_Shdr *)section_header)->sh_name);
  }
  else
  {
    if (specs->e == BIG)
      return (strtab + endian_swap32(((Elf64_Shdr *)section_header)->sh_name));
    else
      return (strtab + ((Elf64_Shdr *)section_header)->sh_name);
  }
}
