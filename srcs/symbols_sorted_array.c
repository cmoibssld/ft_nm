#include <elf.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

#include "endian.h"
#include "filling_array.h"
#include "identification.h"
#include "section_header_info.h"
#include "sorting.h"
#include "symbols_sorted_array.h"

// On the method. Allow a table of pointer onto Symbols headers. Then sort it, so it's like nm output.
// Since file in in memory, looping twice (one to know the total size of the array, the second one to fill it) is not important. Twice, yes but one, knowned-before allocation.
// Better than malloc and realloc. 

ssize_t  looping_on_sections(const char *loaded_file, const size_t loaded_size, const t_spec *specs, const t_section_table_data *info)
{
  size_t      total_symbols;
  size_t      i;
  size_t      symbol_size;
  uint32_t    sh_type;
  const void  *section_header;
  
  total_symbols = 0;
  i = 0;
  symbol_size = specs->arch == X32_BIT ? sizeof(Elf32_Sym) : sizeof(Elf64_Sym);

  while (i < info->total_entry)
  {
    if (loaded_size <= info->address + (i + 1) * symbol_size) // Equal to loaded_size cannot be reached
      return (-1);                                            // i + 1 because symbol must fit entirely
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
  uint64_t  section_size; // Elf64_Shdr->sh_size is an Elf64_Xword so uint64_t. Not optimal for 32 bits but whatever.
  size_t    i;
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
    // Identify if symbols is processor/OS specific skip it ?
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
    return (NULL); // out of bound
  else
    *total_symbols = (size_t)symbols_count;
  // ptr_symbol_size = specs->arch == X32_BIT ? sizeof(Elf32_Sym *) : sizeof(Elf64_Sym *);
  ophelia = (s_symbol *)malloc(*total_symbols * sizeof(s_symbol));
  if (ophelia == NULL)
    return (NULL); // two types of error... change this
  if (fill_array_per_section(ophelia, loaded_file, specs, info) != FILLING_OK)
  {
    // but never happens... There is no allocation memory, out of bound is verified before... Maybe non-null terminated string?
    free(ophelia);
    return (NULL);
  }
  return (ophelia);
}

void sort_array(s_symbol *symbols_array, const size_t total_symbols, const t_options *opt)
{
  if (opt->p == true)
    return ;
  if (opt->r == false)
    qsort(symbols_array, total_symbols, sizeof(s_symbol), sym_compare); // total_symbols => because sizeof(symbols_array) / sizeof(symbols_array[0]) = total_symbols
  else
    qsort(symbols_array, total_symbols, sizeof(s_symbol), rev_sym_cmp);
  // printf("Sorting as took place\n");
}

// Go for output formating. This function is to be only use for debugging after refactor of the fil part
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
