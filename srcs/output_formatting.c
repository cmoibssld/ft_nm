#include <elf.h>
#include <inttypes.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "output_formatting.h"
#include "endian.h"
#include "identification.h"
#include "main.h"

// New print function
// For function, initialized function or else, I need to look at the section...

char  get_sym_flags(const char bind, const char type, const uint16_t st_shndx, const uint32_t sh_type, const uint64_t sh_flags)
// uint64_t not optimal for flags in 32bits -> uint32_t but whatever, that's not that important
{
  char   c;

  if (bind == STB_GNU_UNIQUE)
    c = 'u';
  else if (bind == STB_WEAK)
  {
    c = 'W';
    if (st_shndx == SHN_UNDEF)
      c = 'w';
  }
  else if (bind == STB_WEAK && type == STT_OBJECT)
  {
    c = 'V';
    if (st_shndx == SHN_UNDEF)
      c = 'v';
  }
  else if (st_shndx == SHN_UNDEF)
    c = 'U';
  else if (st_shndx == SHN_ABS)
    c = 'A';
  else if (st_shndx == SHN_COMMON)
    c = 'C';
  else if (type == SHT_NOBITS && sh_flags == (SHF_ALLOC | SHF_WRITE))
    c = 'B';
  else if (sh_type == SHT_PROGBITS && sh_flags == SHF_ALLOC)
    c = 'R'; // not sure ?
  else if (sh_type == SHT_PROGBITS && sh_flags == (SHF_ALLOC | SHF_WRITE))
    c = 'D';
  else if (sh_type == SHT_PROGBITS && sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
    c = 'T';
  else if (sh_type == SHT_DYNAMIC)
    c = 'D';
  else if (sh_type == SHT_PROGBITS && sh_flags == SHF_MASKPROC)
    c = 'G'; // not sure either. Only in .got that global variable are mention and G is for stuff like global var
  else
    c = '?';

  if (bind == STB_LOCAL && c != '?')
    c += 32;
  
  if (sh_type == SHT_PROGBITS && sh_flags == 0)
    c = 'N';

  // remaining: i . I . N . n . p . S/s  . - .
  // i is not standart uni
  return (c);
}

int  print_x32(const Elf32_Sym *sym, const Elf32_Shdr *section, const char *name, const t_elf_endian e, const t_options *opt)
{
  int         res;
  char        letter;
  uint32_t    addr;
  const char  bind = ELF32_ST_BIND(sym->st_info);
  const char  type = ELF32_ST_TYPE(sym->st_info);

  (void)section;
  (void)opt;
  if (bind >= STB_LOPROC && bind <= STB_HIPROC)
    return (0);
  letter = get_sym_flags(bind, type, sym->st_shndx, section->sh_type, section->sh_flags);
  // if (letter == '\0')
  //   letter = look_for_something_else(sym, e, X32_BIT, opt);      
  if (opt->a == false && letter == 'N')
    return (0);
  addr = e == LITTLE ? sym->st_value : endian_swap32(sym->st_value);
  if (letter == 'U' || letter == 'u')
    res = printf("%18c %s\n", letter, name);
  else
    res = (printf("%016X %c %s\n", addr, letter, name) == -1);
  return (res);
}      

int  print_x64(const Elf64_Sym *sym, const Elf64_Shdr *section, const char *name, const t_elf_endian e, const t_options *opt)
{
  int         res;
  char        letter;
  uint64_t    addr;
  const char  bind = ELF64_ST_BIND(sym->st_info);
  const char  type = ELF64_ST_TYPE(sym->st_info);

  (void)section;
  (void)opt;
  if (bind >= STB_LOPROC && bind <= STB_HIPROC)
    return (0);
  letter = get_sym_flags(bind, type, sym->st_shndx, section->sh_type, section->sh_flags);
  // if (letter == '\0')
  //   letter = look_for_something_else(sym, e, X64_BIT, opt);
  addr = e == LITTLE ? sym->st_value : endian_swap64(sym->st_value);
  if (letter == 'U' || letter == 'u')
    res = printf("%18c %s\n", letter, name);
  else
    res = printf("%016" PRIx64 " %c %s\n", addr, letter, name);
  printf("flags: %lu  ", section->sh_flags);
  return (res);
}

int  print_array(const s_symbol *symbols, const size_t total_symbols, const t_spec *specs, const t_options *opt)
{
  size_t    i;
  int      value;

  i = 0;
  while (i < total_symbols)
  {
    if (specs->arch == X32_BIT)
      value = print_x32(symbols[i].sym, symbols[i].section, symbols[i].name, specs->e, opt);  
    else
      value = print_x64(symbols[i].sym, symbols[i].section, symbols[i].name, specs->e, opt);
    // const int byt = printf("%s %s\n", value, symbols[i].name);
    if (value == -1)
      return (-1);
    // const size_t bytes = byt;
    // if (bytes != ft_strlen(value) + ft_strlen(" \n") + ft_strlen(symbols[i].name))
    // {
    //   perror("unexpected i/o outcome");
    //   return (1);
    // }
    ++i;
  }
  return (0);
}
