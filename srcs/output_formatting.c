#include <elf.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "endian.h"
#include "identification.h"
#include "libft.h"
#include "main.h"
#include "output_formatting.h"

bool is_a_bonus(const char c, const uint64_t sh_flags)
{
  if (c == 'a' || c == 'A')
    return (true);
  else if (c == 'N')
    return (true);
  else if (c == 't' && sh_flags == SHF_EXECINSTR)
    return (true);
  else if (c == '?')
    return (true);
  else
    return (false);
}

static bool is_local_symbol(const char c)
{
  return ((c >= 'a') && (c <= 'z'));
}

static bool is_undefined(const char c)
{
  return (c == 'U' || c == 'u' || c == 'w' || c == 'v');
}

char get_sym_flags(const char bind, const char type, const uint16_t st_shndx,
                   const uint32_t sh_type, const uint64_t sh_flags)
// uint64_t not optimal for flags in 32bits -> uint32_t but whatever, that's not
// that important
{
  char c;

  if (bind == STB_GNU_UNIQUE)
    c = 'u';
  else if (bind == STB_WEAK) {
    c = 'W';
    if (st_shndx == SHN_UNDEF)
      c = 'w';
  } else if (bind == STB_WEAK && type == STT_OBJECT) {
    c = 'V';
    if (st_shndx == SHN_UNDEF)
      c = 'v';
  } else if (st_shndx == SHN_UNDEF)
    c = 'U';
  else if (st_shndx == SHN_ABS)
    c = 'A';
  else if (st_shndx == SHN_COMMON)
    c = 'C';
  else if (sh_type == SHT_NOBITS && sh_flags == (SHF_ALLOC | SHF_WRITE))
    c = 'B';
  else if (sh_type == SHT_PROGBITS && sh_flags == SHF_ALLOC)
    c = 'R'; // not working !!
  else if (sh_type == SHT_PROGBITS && sh_flags == (SHF_ALLOC | SHF_WRITE))
    c = 'D'; // not working !!
  else if (sh_type == SHT_PROGBITS && sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
    c = 'T';
  else if (sh_type == SHT_DYNAMIC)
    c = 'D';
  else if (sh_type == SHT_PROGBITS && sh_flags == SHF_MASKPROC)
    c = 'G'; // not sure either. Only in .got that global variable are mention
             // and G is for stuff like global var
  else if (sh_type == SHT_PROGBITS)
    c = 'N'; // not perfect I guess
  else if (sh_type == SHT_NOTE)
    c = 'R';
  else if (sh_type == SHT_INIT_ARRAY ||
           sh_type == SHT_FINI_ARRAY) // constructor for C++
    c = 'D';
  else
    c = '?';

  if (bind == STB_LOCAL && c != '?' && c != 'N')
    c = ft_tolower(c);

  // remaining: i . I . n . p . S/s  . - .
  // i is not standart uni
  return (c);
}

int print_x32(const Elf32_Sym *sym, const Elf32_Shdr *section, const char *name, const t_elf_endian e, const t_options *opt)
{
  int        res;
  char       letter;
  uint32_t   addr;
  
  const char      bind = ELF32_ST_BIND(sym->st_info);
  const char      type = ELF32_ST_TYPE(sym->st_info);
  const uint16_t  st_shndx = e == LITTLE ? sym->st_shndx : endian_swap16(sym->st_shndx);

  const uint32_t  sh_type =
    st_shndx == SHN_ABS ? 0 :
                        e == LITTLE ? section->sh_type : endian_swap16(section->sh_type);
  const uint64_t  sh_flags =
    st_shndx == SHN_ABS ? 0 :
                        e == LITTLE ? section->sh_flags : endian_swap64(section->sh_flags);
  
  letter = get_sym_flags(bind, type, st_shndx, sh_type, sh_flags);
  addr = e == LITTLE ? sym->st_value : endian_swap32(sym->st_value);
  // if (opt->a == false && (is_a_bonus(letter, sh_flags)))
  //   return (0);
  if (opt->g == true && is_local_symbol(letter))
    return (0);
  if (opt->u == true && is_undefined(letter) == false)
    return (0);
  // if (name && name[0] == '$')
    // printf("type: %d, bind: %d ---", type, bind);
  if (is_undefined(letter) == true)
    res = printf("%18c %s\n", letter, name);
  else
    res = (printf("%016X %c %s\n", addr, letter, name) == -1);
  return (res);
}

int print_x64(const Elf64_Sym *sym, const Elf64_Shdr *section, const char *name, const t_elf_endian e, const t_options *opt)
{
  int        res;
  char       letter;
  uint64_t   addr;
  
  const char      bind = ELF32_ST_BIND(sym->st_info);
  const char      type = ELF32_ST_TYPE(sym->st_info);
  const uint16_t  st_shndx = e == LITTLE ? sym->st_shndx : endian_swap16(sym->st_shndx);

  const uint32_t  sh_type =
    st_shndx == SHN_ABS ? 0 :
                        e == LITTLE ? section->sh_type : endian_swap16(section->sh_type);
  const uint64_t  sh_flags =
    st_shndx == SHN_ABS ? 0 :
                        e == LITTLE ? section->sh_flags : endian_swap64(section->sh_flags);

  if (sh_type >= SHT_LOOS && sh_type <= SHT_HIPROC)
    return (0);
  letter = get_sym_flags(bind, type, st_shndx, sh_type, sh_flags);
  addr = e == LITTLE ? sym->st_value : endian_swap64(sym->st_value);
  // if (opt->a == false && (is_a_bonus(letter, sh_flags)))
  //   return (0);
  if (opt->g == true && is_local_symbol(letter))
    return (0);
  if (opt->u == true && is_undefined(letter) == false)
    return (0);
  // if (name && (name[0] == '$' || name[0] == '\0'))
  //   return (0);
  if (is_undefined(letter) == true)
    res = printf("%18c %s\n", letter, name);
  else
    res = printf("%016" PRIx64 " %c %s\n", addr, letter, name);
  return (res);
}

int print_array(const s_symbol *symbols, const size_t total_symbols, const t_spec *specs, const t_options *opt)
{
  size_t i;
  int value;

  i = 0;
  while (i < total_symbols)
  {
    if (specs->arch == X32_BIT)
      value = print_x32(symbols[i].sym, symbols[i].section,
                        symbols[i].name == NULL ? "" : symbols[i].name,
                        specs->e, opt);
    else
      value = print_x64(symbols[i].sym, symbols[i].section,
                        symbols[i].name == NULL ? "" : symbols[i].name,
                        specs->e, opt);
    if (value == -1)
      return (-1);
    ++i;
  }
  return (0);
}
