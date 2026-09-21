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

static bool is_a_bonus(const char c)
{
  if (c == 'a' || c == 'A')
    return (true);
  else if (c == 'N')
    return (true);
  else if (c == 't')
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

  // if (c == '?')
  // printf("sh_type: %d, sh_flags: %lu, st_shndx: %d --", sh_type, sh_flags, st_shndx);

  // remaining: i . I . n . p . S/s  . - .
  // i is not standart uni
  // type 14 and 15
  return (c);
}

int print_x32(const Elf32_Sym *sym, const Elf32_Shdr *section, const char *name,
              const t_elf_endian e, const t_options *opt) {
  int res;
  char letter;
  uint32_t addr;
  const char bind = ELF32_ST_BIND(sym->st_info);
  const char type = ELF32_ST_TYPE(sym->st_info);

  if (bind >= STB_LOPROC && bind <= STB_HIPROC)
    return (0);
  // invalid read, on flags IMO not on the type but ...
  if (sym->st_shndx ==
      SHN_ABS) // cannot read the header it seems -> invalid read...
    letter = get_sym_flags(bind, type, sym->st_shndx, 0, 0);
  else
    letter = get_sym_flags(bind, type, sym->st_shndx, section->sh_type,
                           section->sh_flags);
  if (opt->a == false && is_a_bonus(letter))
    return (0);
  if (opt->g == true && is_local_symbol(letter))
    return (0);
  if (name != NULL && name[0] == '$')
    return (0);
  addr = e == LITTLE ? sym->st_value : endian_swap32(sym->st_value);
  if (letter == 'U' || letter == 'u' || letter == 'w')
    res = printf("%18c %s\n", letter, name);
  else
    res = (printf("%016X %c %s\n", addr, letter, name) == -1);
  return (res);
}

int print_x64(const Elf64_Sym *sym, const Elf64_Shdr *section, const char *name,
              const t_elf_endian e, const t_options *opt) {
  int res;
  char letter;
  uint64_t addr;
  const char bind = ELF64_ST_BIND(sym->st_info);
  const char type = ELF64_ST_TYPE(sym->st_info);

  if (bind >= STB_LOPROC && bind <= STB_HIPROC)
    return (0);
  if (sym->st_shndx ==
      SHN_ABS) // cannot read the header it seems -> invalid read...
    letter = get_sym_flags(bind, type, sym->st_shndx, 0, 0);
  else
    letter = get_sym_flags(bind, type, sym->st_shndx, section->sh_type,
                           section->sh_flags);
  if (opt->a == false && is_a_bonus(letter))
    return (0);
  if (opt->g == true && is_local_symbol(letter))
    return (0);
  if (name != NULL && name[0] == '$') // for weird symbols created by aarch64
    return (0);
  addr = e == LITTLE ? sym->st_value : endian_swap64(sym->st_value);
  if (letter == 'U' || letter == 'u' || letter == 'w')
    res = printf("%18c %s\n", letter, name);
  else
    res = printf("%016" PRIx64 " %c %s\n", addr, letter, name);
  return (res);
}

int print_array(const s_symbol *symbols, const size_t total_symbols,
                const t_spec *specs, const t_options *opt) {
  size_t i;
  int value;

  i = 0;
  while (i < total_symbols) {
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
