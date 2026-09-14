#include <elf.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include "output_formatting.h"
#include "section_header_table.h"

t_section_table_status  print_function_symbol(const uint8_t binding, const bool little_endian, const bool x32, const void *symbol_header, const char *string_table)
{
  char symbol;
  const char *name;

  if (binding == STB_LOCAL)
    symbol = 't';
  else
    symbol = 'T';
  if (x32 == true)
  {
    name = string_table + ((const Elf32_Sym *)symbol_header)->st_name;
    if (little_endian == false)
      printf("%016X %c %s\n", endian_swap32(((const Elf32_Sym *)symbol_header)->st_value), symbol, name);
    else
      printf("%016X %c %s\n", ((const Elf32_Sym *)symbol_header)->st_value, symbol, name);
  }
  else
  {
    name = string_table + ((const Elf64_Sym *)symbol_header)->st_name;
    if (little_endian == false)
      printf("%016" PRIx64 " %c %s\n", endian_swap64(((const Elf64_Sym *)symbol_header)->st_value), symbol, name);
    else
      printf("%016" PRIx64" %c %s\n", ((const Elf64_Sym *)symbol_header)->st_value, symbol, name);
    }
    return (CORRECT);
  }

t_section_table_status  print_undefined_symbol(const bool little_endian, const bool x32, const void *symbol_header, const char *string_table)
{
  char symbol;
  uint32_t  st_name;

  symbol = 'U';
  if (x32 == true)
    st_name = ((const Elf32_Sym *)symbol_header)->st_name;
  else
    st_name = ((const Elf64_Sym *)symbol_header)->st_name;
  if (!little_endian)
    st_name = endian_swap32(st_name);
  // printf("st_name=%u, strtab=%p, name='%s'\n",
       // st_name, (void *)string_table, string_table + st_name);
  printf("%18c %s\n", symbol, string_table + st_name);
  return (CORRECT);
}

// Check that st_name is < strtab_size and that it's null terminated...
