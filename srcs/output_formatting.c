#include <inttypes.h>
#include <stdio.h>

#include "output_formatting.h"
#include "section_header_table.h"

t_section_table_status  print_function_symbol(const uint8_t binding, const bool little_endian, const bool x32, const void *symbol_header)
{
  char symbol;
  
  if (binding == STB_LOCAL)
    symbol = 't';
  else
    symbol = 'T';
  if (x32 == true)
  {
    if (little_endian == false)
      printf("%016X %c\n", endian_swap32(((const Elf32_Sym *)symbol_header)->st_value), symbol);
    else
      printf("%016X %c\n", ((const Elf32_Sym *)symbol_header)->st_value, symbol);
  }
  else
  {
    if (little_endian == false)
      printf("%016" PRIx64 " %c\n", endian_swap64(((const Elf64_Sym *)symbol_header)->st_value), symbol);
    else
      printf("%016" PRIx64" %c\n", ((const Elf64_Sym *)symbol_header)->st_value, symbol);
    }
    return (CORRECT);
  }
