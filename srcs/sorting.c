#include <limits.h>
#include <stddef.h>

#include "libft.h"
#include "sorting.h"
#include "symbols_sorted_array.h"

static const char *sym_trim(const char *str)
{
  unsigned int  i;

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
// - at '@' are like 255
 
static int  letter_only_cmp(const char *s1, const char *s2)
{
  size_t i;

  i = 0;
  while (i < ft_strlen(s1) && i < ft_strlen(s2))
  {
    if ((s1[i] != s2[i]) && (s1[i] == '@' || s2[i] == '@'))
      return (s1[i] == '@' ? 255 - s2[i] : s1[i] - 255); // so weird but works
    if (ft_tolower(s1[i]) != ft_tolower(s2[i]))
      break ;
    ++i;
  }
  return (ft_tolower(s1[i]) - ft_tolower(s2[i]));
}

int  sym_compare(const void *sym_1, const void *sym_2)
{
  const char *s1;
  const char *s2;

  s1 = sym_trim(((const s_symbol *)sym_1)->name);
  s2 = sym_trim(((const s_symbol *)sym_2)->name);
  
  return (letter_only_cmp(s1, s2));
}

int  rev_sym_cmp(const void *sym_1, const void *sym_2)
{
  return (-(sym_compare(sym_1, sym_2)));
}
