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
#include "libft.h"
#include "section_header_info.h"
#include "sorting.h"
#include "symbols_sorted_array.h"

// On the method. Allow a table of pointer onto Symbols headers. Then sort it,
// so it's like nm output. Since file in in memory, looping twice (one to know
// the total size of the array, the second one to fill it) is not important.
// Twice, yes but one, knowned-before allocation. Better than malloc and
// realloc.

ssize_t looping_on_sections(const char *loaded_file, const size_t loaded_size,
                            const t_spec *specs,
                            const t_section_table_data *info)
{
  size_t total_symbols;
  size_t i;
  size_t symbol_size;
  uint32_t sh_type;
  const void *section_header;

  total_symbols = 0;
  i = 0;
  symbol_size = specs->arch == X32_BIT ? sizeof(Elf32_Sym) : sizeof(Elf64_Sym);

  while (i < info->total_entry)
  {
    if (loaded_size <=
        info->address +
            (i + 1) * symbol_size) // Equal to loaded_size cannot be reached
      return (-1);                 // i + 1 because symbol must fit entirely
    section_header =
        specs->arch == X32_BIT
            ? (void *)&((Elf32_Shdr *)(loaded_file + info->address))[i]
            : (void *)&((Elf64_Shdr *)(loaded_file + info->address))[i];
    sh_type = specs->arch == X32_BIT ? ((Elf32_Shdr *)section_header)->sh_type
                                     : ((Elf64_Shdr *)section_header)->sh_type;
    sh_type = specs->e == LITTLE ? sh_type : endian_swap32(sh_type);
    if (sh_type == SHT_SYMTAB)
      total_symbols +=
          looping_on_symbols(section_header, loaded_size, specs, symbol_size);
    // need a good way to add + verify if looping is -1...
    ++i;
  }
  return (total_symbols);
}

ssize_t looping_on_symbols(const void *section_header, const size_t loaded_size,
                           const t_spec *specs, const size_t symbol_size)
{
  uint64_t section_size; // Elf64_Shdr->sh_size is an Elf64_Xword so uint64_t.
                         // Not optimal for 32 bits but whatever.
  size_t i;
  bool out_of_bound;

  section_size = specs->arch == X32_BIT
                     ? ((Elf32_Shdr *)section_header)->sh_size
                     : ((Elf64_Shdr *)section_header)->sh_size;
  section_size =
      specs->e == LITTLE ? section_size : endian_swap32(section_size);
  i = 0;
  while (i * symbol_size < section_size)
  {
    out_of_bound = true;
    if (specs->arch == X32_BIT)
    {
      out_of_bound =
          specs->e == LITTLE
              ? ((Elf32_Shdr *)section_header)->sh_offset + i * symbol_size >
                    loaded_size
              : endian_swap32(((Elf32_Shdr *)section_header)->sh_offset) +
                        i * symbol_size >
                    loaded_size;
    }
    else if (specs->arch == X64_BIT)
    {
      out_of_bound =
          specs->e == LITTLE
              ? ((Elf64_Shdr *)section_header)->sh_offset + i * symbol_size >
                    loaded_size
              : endian_swap64(((Elf64_Shdr *)section_header)->sh_offset) +
                        i * symbol_size >
                    loaded_size;
    }
    if (out_of_bound == true)
      return (-1);
    // Identify if symbols is processor/OS specific skip it ?
    ++i;
  }
  return (i - 1); // first of the section is always a null symbol
}

s_symbol *create_array(const char *loaded_file, const size_t loaded_size,
                       const t_spec *specs, const t_section_table_data *info,
                       size_t *total_symbols)
{
  s_symbol *ophelia;
  ssize_t symbols_count;

  symbols_count = looping_on_sections(loaded_file, loaded_size, specs, info);
  if (symbols_count == -1)
    return (NULL); // out of bound
  else
    *total_symbols = (size_t)symbols_count;
  ophelia = (s_symbol *)ft_calloc(*total_symbols, sizeof(s_symbol));
  if (ophelia == NULL)
    return (NULL); // two types of error... change this
  if (fill_array_per_section(ophelia, loaded_file, specs, info) != FILLING_OK)
  {
    free(ophelia);
    return (NULL);
  }
  if (check_strings_name(ophelia, *total_symbols, loaded_file, loaded_size) ==
      false) {
    free(ophelia);
    return (NULL);
  }
  return (ophelia);
}

bool check_strings_name(const s_symbol *symbol_array,
                        const size_t total_symbols, const char *loaded_file,
                        const size_t loaded_size)
{
  uint64_t i;
  const char *end;

  end = loaded_file + loaded_size;
  for (size_t sym = 0; sym < total_symbols; ++sym)
  {
    if (symbol_array[sym].name == NULL)
      continue;
    i = 0;
    while (symbol_array[sym].name[i] != '\0')
    {
      if (&symbol_array[sym].name[i] == end)
      {
        perror("string name out of bound: ");
        return (false);
      }
      ++i;
    }
    for (size_t j = 0; j < total_symbols; ++j)
    {
      if (j == sym ||
           symbol_array[j].name == NULL)
        continue;
    }
  }
  return (true);
}

void sort_array(s_symbol *symbols_array, const size_t total_symbols,
                const t_spec *specs, const t_options *opt)
{
  if (opt->p == true)
    return;
  if (opt->r == false)
  {
    if (specs->arch == X32_BIT)
      qsort(symbols_array, total_symbols, sizeof(s_symbol), sym_compare32); // total_symbols => because sizeof(symbols_array) /
    else
      qsort(symbols_array, total_symbols, sizeof(s_symbol), sym_compare64);
    // sizeof(symbols_array[0]) = total_symbols
  }
  else
  {
    if (specs->arch == X32_BIT)
      qsort(symbols_array, total_symbols, sizeof(s_symbol), rev_sym_cmp32);
    else
      qsort(symbols_array, total_symbols, sizeof(s_symbol), rev_sym_cmp64);
  }
}
