#include <elf.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

#include "endian.h"
#include "identification.h"
#include "section_header_info.h"
#include "section_header_table.h"
#include "sorting.h"
#include "symbols_sorted_array.h"

// On the method. Allow a table of pointer onto Symbols headers. Then sort it, so it's like nm output.
// Since file in in memory, looping twice (one to know the total size of the array, the second one to fill it) is not important. Twice, yes but one, knowned-before allocation.
// Better than malloc and realloc. 

ssize_t  looping_on_sections(const char *loaded_file, const size_t loaded_size, const t_spec *specs, const t_section_table_data *info)
{
  size_t      total_symbols;
  uint16_t    i;
  size_t      symbol_size;
  uint32_t    sh_type;
  const void  *section_header;
  
  total_symbols = 0;
  i = 0;
  symbol_size = specs->arch == X32_BIT ? sizeof(Elf32_Sym) : sizeof(Elf64_Sym);

  while (i < info->total_entry)
  {
    if (loaded_size < info->address + i * symbol_size)
      return (-1);
    // Check if header is symbols aka SHT_SYMTAB
    section_header = specs->arch == X32_BIT ? (void *)&((Elf32_Shdr *)(loaded_file + info->address))[i] : (void *)&((Elf64_Shdr *)(loaded_file + info->address))[i];
    sh_type = specs->arch == X32_BIT ? ((Elf32_Shdr *)section_header)->sh_type : ((Elf64_Shdr *)section_header)->sh_type;
    sh_type = specs->e == LITTLE ? sh_type : endian_swap32(sh_type);
    if (sh_type == SHT_SYMTAB)
      total_symbols += looping_on_symbols(section_header, loaded_size, specs, symbol_size);
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

s_symbol  *create_array(const char *loaded_file, const size_t loaded_size, const t_spec *specs, const t_section_table_data *info, size_t *total_symbols)
{
  s_symbol *ophelia;
  ssize_t  symbols_count;
  // size_t   ptr_symbol_size;

  symbols_count = looping_on_sections(loaded_file, loaded_size, specs, info);
  if (symbols_count == -1)
    return (NULL);
  else
    *total_symbols = symbols_count;
  // ptr_symbol_size = specs->arch == X32_BIT ? sizeof(Elf32_Sym *) : sizeof(Elf64_Sym *);
  ophelia = (s_symbol *)malloc(*total_symbols * sizeof(s_symbol));
  if (ophelia == NULL)
    return (NULL);
  fill_array(ophelia, loaded_file, specs, info); // limits where tested before, no need for re-check during second passinge
  return (ophelia);
}

void  fill_array(s_symbol *symbol_array, const char *loaded_file, const t_spec *specs, const t_section_table_data *info)
{
  uint16_t    i;
  uint16_t    symbol_idx_in_section;
  uint16_t    symbol_array_idx;
  uint16_t    sh_type;
  size_t      section_size;
  size_t      symbol_size;
  const char  *string_table;
  const void  *section_header;

  i = 0;
  symbol_array_idx = 0;
  section_header = 0;
  symbol_size = specs->arch == X32_BIT ? sizeof(Elf32_Sym) : sizeof(Elf64_Sym);
  while (i < info->total_entry)
  {
   section_header = specs->arch == X32_BIT ? (void *)&((Elf32_Shdr *)(loaded_file + info->address))[i] : (void *)&((Elf64_Shdr *)(loaded_file + info->address))[i];
    sh_type = specs->arch == X32_BIT ? ((Elf32_Shdr *)section_header)->sh_type : ((Elf64_Shdr *)section_header)->sh_type;
    sh_type = specs->e == LITTLE ? sh_type : endian_swap32(sh_type);
    if (sh_type == SHT_SYMTAB || sh_type == SHT_DYNSYM)
    // reached a section with symbol, copy the address of the pointer into the symbol_array
    {
      section_size = specs->arch == X32_BIT ? ((Elf32_Shdr *)section_header)->sh_size : ((Elf64_Shdr *)section_header)->sh_size;
      string_table = get_string_table(loaded_file, section_header, specs, info);
      symbol_idx_in_section = 1; // index 0 is always a NULL symbol
      while (symbol_idx_in_section * symbol_size < section_size)
      {
        if (specs->arch == X32_BIT)
        {
          symbol_array[symbol_array_idx].sym = specs->e == LITTLE ? &((Elf32_Sym *)(loaded_file + ((Elf32_Shdr *)section_header)->sh_offset))[symbol_idx_in_section] : &((Elf32_Sym *)(loaded_file + endian_swap32(((Elf32_Shdr *)section_header)->sh_offset)))[symbol_idx_in_section];
          symbol_array[symbol_array_idx].name = specs->e == LITTLE ? string_table + ((Elf32_Sym *)(symbol_array[symbol_array_idx].sym))->st_name : string_table +endian_swap32(((Elf32_Sym *)(symbol_array[symbol_array_idx].sym))->st_name);
        }
        else {
          symbol_array[symbol_array_idx].sym = specs->e == LITTLE ? &((Elf64_Sym *)(loaded_file + ((Elf64_Shdr *)section_header)->sh_offset))[symbol_idx_in_section] : &((Elf64_Sym *)(loaded_file + endian_swap64(((Elf64_Shdr *)section_header)->sh_offset)))[symbol_idx_in_section];
          symbol_array[symbol_array_idx].name = specs->e == LITTLE ? string_table + ((Elf64_Sym *)(symbol_array[symbol_array_idx].sym))->st_name : string_table + endian_swap32(((Elf64_Sym *)(symbol_array[symbol_array_idx].sym))->st_name);
          //assign name -> no copy of the string
        }
        ++symbol_idx_in_section;
        ++symbol_array_idx;
      }
    }
    ++i;
  } 
}

void  sort_array(s_symbol *symbols_array, const t_spec *specs, const size_t total_symbols, const t_options *opt)
{
  size_t  symbol_size;
  size_t  array_size;

  (void)total_symbols;
  if (opt->p == true)
    return ;
  symbol_size = specs->arch == X32_BIT ? sizeof(Elf32_Sym *) : sizeof(Elf64_Sym *);
  array_size = specs->arch == X32_BIT ? sizeof(Elf32_Sym **) : sizeof(Elf64_Sym **);
  if (opt->r == false)
    qsort(&symbols_array, array_size/symbol_size, symbol_size, sym_compare);
  else
    qsort(&symbols_array, array_size/symbol_size, symbol_size, rev_sym_cmp);
  printf("Sorting as took place\n");
}

void  print_array_important_stuff(s_symbol *symbols_array, const t_spec *specs, const size_t total_symbols)
{
  size_t    idx;
  uint8_t   st_info;
  uint8_t   type;
  uint8_t   bind;
  uint16_t  st_shndx; // half, word are uint16_t

  idx = 0;
  while (idx < total_symbols)
  {
    st_info = specs->arch == X32_BIT ? ((Elf32_Sym *)symbols_array[idx].sym)->st_info : ((Elf64_Sym *)symbols_array[idx].sym)->st_info;
    st_shndx = specs->arch == X32_BIT ? ((Elf32_Sym *)symbols_array[idx].sym)->st_shndx : ((Elf64_Sym *)symbols_array[idx].sym)->st_shndx;

    st_shndx = specs->e == LITTLE ? st_shndx : endian_swap16(st_shndx);

    bind = specs->arch == X32_BIT ? ELF32_ST_BIND(st_info) : ELF64_ST_BIND(st_info);
    if (st_shndx != SHN_UNDEF)
    {
      printf("Symbol is defined: ");
      type = specs->arch == X32_BIT ? ELF32_ST_TYPE(st_info) : ELF64_ST_TYPE(st_info);
      if (type == STT_FUNC)
        printf("T ! it's a function");
      else
        printf("i dunno, something else");
    }
    else if (bind == STB_GLOBAL)
      printf("Symbol is undef: U");
    printf("\tname: %s\n", symbols_array[idx].name); // invalid read. ofcourse... 
    ++idx;
  }
  printf("there were %lu symbols in the array\n", idx);
}
