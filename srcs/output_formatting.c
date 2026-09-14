#include <elf.h>
#include <inttypes.h>
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

// t_section_table_status access_symbol_value_64bits(const bool litlle_endian, const void* symbol_header, const char *loaded_file)
// {
// }
