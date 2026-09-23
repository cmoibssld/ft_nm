#include <elf.h>
#include <limits.h>
#include <stddef.h>
#include <unistd.h>

#include "libft.h"
#include "output_formatting.h"
#include "sorting.h"
#include "symbols_sorted_array.h"

static const char *sym_trim(const char *str)
{
  unsigned int i;

  if (str == NULL)
    return (NULL);
  i = 0;
  while (str[i] != '\0')
  {
    if (i == UINT_MAX) // should lower the limit probably
      return (NULL);
    if (ft_isalpha(str[i]))
      return (&str[i]);
    ++i;
  }
  return (str);
}

// special sort rules (empirical)
// - uppercase and lowercase are treaded as the same
// - at '@' and underscore '_' are ignored

static int letter_only_cmp(const char *s1, const char *s2)
{
  size_t i;
  size_t j;

  i = 0;
  j = 0;
  while (i < ft_strlen(s1) && j < ft_strlen(s2))
  {
    while (s1[i] == '_' || s1[i] == '@')
      ++i;
    while (s2[j] == '_' || s2[j] == '@')
      ++j;
    if (ft_tolower(s1[i]) != ft_tolower(s2[j]))
      break;
    ++i;
    ++j;
  }
  return (ft_tolower(s1[i]) - ft_tolower(s2[j]));
}

static char  get_letter(const void *symbol_hdr, const void *section_hdr, const bool arch_x32)
{
  if (arch_x32 == true)
  {
    Elf32_Sym *sy = (Elf32_Sym *)symbol_hdr;
    Elf32_Shdr *hd = (Elf32_Shdr *)section_hdr;
    if (hd == NULL)
      return (get_sym_flags(ELF32_ST_BIND(sy->st_info), ELF32_ST_TYPE(sy->st_info), sy->st_shndx, 0, 0));
    else
      return (get_sym_flags(ELF32_ST_BIND(sy->st_info), ELF32_ST_TYPE(sy->st_info), sy->st_shndx, hd->sh_type, hd->sh_flags));
  }
  else
  {
    Elf64_Sym *sy = (Elf64_Sym *)symbol_hdr;
    Elf64_Shdr *hd = (Elf64_Shdr *)section_hdr;
    if (hd == NULL)
      return (get_sym_flags(ELF64_ST_BIND(sy->st_info), ELF64_ST_TYPE(sy->st_info), sy->st_shndx, 0, 0));
    else
      return (get_sym_flags(ELF64_ST_BIND(sy->st_info), ELF64_ST_TYPE(sy->st_info), sy->st_shndx, hd->sh_type, hd->sh_flags));
  }
}


int sym_compare32(const void *sym_1, const void *sym_2)
{
  const char *s1;
  const char *s2;
  int res;

  s1 = sym_trim(((const s_symbol *)sym_1)->name);
  s2 = sym_trim(((const s_symbol *)sym_2)->name);
  if (s1 == NULL || s2 == NULL)
    return (s1 == NULL ? 0 : 1);

  res = letter_only_cmp(s1, s2);
  if (res == 0)
  {
    const Elf32_Shdr *hd = (Elf32_Shdr *)((s_symbol *)sym_1)->section;
    const Elf32_Sym  *sy = (Elf32_Sym *)((s_symbol *)sym_1)->sym;
    char c1 = get_letter(sy, hd, true);
    hd = (Elf32_Shdr *)((s_symbol *)sym_2)->section;
    sy = (Elf32_Sym *)((s_symbol *)sym_2)->sym;
    char c2 = get_letter(sy, hd, true);
    res = ft_tolower(c1) - ft_tolower(c2);
    if (res == 0)
    {
      const Elf32_Sym *sy1 = (Elf32_Sym *)((s_symbol *)sym_1)->sym; 
      const Elf32_Sym *sy2 = (Elf32_Sym *)((s_symbol *)sym_2)->sym; 
      res = sy1->st_value - sy2->st_value;
    }
  }
  return (res);
}

int sym_compare64(const void *sym_1, const void *sym_2)
{
  const char *s1;
  const char *s2;
  int        res;

  s1 = sym_trim(((const s_symbol *)sym_1)->name);
  s2 = sym_trim(((const s_symbol *)sym_2)->name);
  if (s1 == NULL || s2 == NULL)
    return (s1 == NULL ? -1 : 1);

  res = letter_only_cmp(s1, s2);
  if (res == 0)
  {
    // printf("Comparing letters: %s, %s... -- ", s1, s2);
    const Elf64_Shdr *hd = (Elf64_Shdr *)((s_symbol *)sym_1)->section;
    const Elf64_Sym  *sy = (Elf64_Sym *)((s_symbol *)sym_1)->sym;
    char c1 = get_letter(sy, hd, false);
    hd = (Elf64_Shdr *)((s_symbol *)sym_2)->section;
    sy = (Elf64_Sym *)((s_symbol *)sym_2)->sym;
    char c2 = get_letter(sy, hd, false);
    // printf("letter %c vs %c\n", c1, c2);
    res = ft_tolower(c1) - ft_tolower(c2);
    if (res == 0)
    {
      const Elf64_Sym *sy1 = (Elf64_Sym *)((s_symbol *)sym_1)->sym; 
      const Elf64_Sym *sy2 = (Elf64_Sym *)((s_symbol *)sym_2)->sym; 
      res = sy1->st_value - sy2->st_value;
    }
  }
  return (res);
}

int rev_sym_cmp32(const void *sym_1, const void *sym_2)
{
  return (-(sym_compare32(sym_1, sym_2)));
}

int rev_sym_cmp64(const void *sym_1, const void *sym_2)
{
  return (-(sym_compare64(sym_1, sym_2)));
}
