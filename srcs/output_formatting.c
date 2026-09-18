#include <elf.h>
#include <inttypes.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

#include "output_formatting.h"
#include "identification.h"
#include "main.h"
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


// New print function
// For function, initialized function or else, I need to look at the section...

#include "libft.h"

char  get_sym_sym(const char bind, const char type)
{
  if (bind == STB_WEAK)
    return (type == STT_OBJECT || type == STT_FUNC ? 'v' : 'w'); // more info, can be upper or lower
  if (type == STT_NOTYPE)
    return ('U'); // 'u' is a GNU extension to ELF symbol bindings
  else if (type == STT_OBJECT)
    return (type == STB_LOCAL ? 'd' : 'D');
  else if (type == STT_FUNC)
    return (bind == STB_LOCAL ? 't' : 'T');
  return (0);
}

const char  *print_x32(const Elf32_Sym *sym, const Elf32_Shdr *section,const t_elf_endian e, const t_options *opt)
{
  char        letter;
  const char  bind = ELF32_ST_BIND(sym->st_info);
  const char  type = ELF32_ST_TYPE(sym->st_info);

  letter = get_sym_sym(bind, type);
  if (letter == '\0')
    letter = look_for_something_else(sym, e, X32_BIT, opt);      
}

const char  *print_x64(const Elf64_Sym *sym, const Elf64_Shdr *section, const t_elf_endian e, const t_options *opt)
{
  char        letter;
  const char  bind = ELF64_ST_BIND(sym->st_info);
  const char  type = ELF64_ST_TYPE(sym->st_info);

  letter = get_sym_sym(bind, type);
  if (letter == '\0')
    letter = look_for_something_else(sym, e, X64_BIT, opt);
}

int  print_array(const s_symbol *symbols, const void *section, const size_t total_symbols, const t_spec *specs, const t_options *opt)
{
  size_t      i;
  const char  *value;

  i = 0;
  while (i < total_symbols)
  {
    if (specs->arch == X32_BIT)
      value = print_x32(symbols[i].sym, section, specs->e, opt);  
    else
      value = print_x64(symbols[i].sym, section, specs->e, opt);
    const int byt = printf("%s %s\n", value, symbols[i].name);
    if (byt == -1)
      return (-1);
    const size_t bytes = byt;
    if (bytes != ft_strlen(value) + ft_strlen(" \n") + ft_strlen(symbols[i].name))
    {
      perror("unexpected i/o outcome");
      return (1);
    }
  }
  return (0);
}
