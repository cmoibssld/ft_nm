#include "libft.h"
#include "sorting.h"
#include "symbols_sorted_array.h"

int  sym_compare(const void *sym_1, const void *sym_2)
{
  const char *s1;
  const char *s2;

  s1 = ((s_symbol *)sym_1)->name;
  s2 = ((s_symbol *)sym_2)->name;
  printf("Comparing %s and %s\n", s1, s2);
  
  return (ft_strncmp(s1, s2, ft_strlen(s1)));
}

int  rev_sym_cmp(const void *sym_1, const void *sym_2)
{
  return (-(sym_compare(sym_1, sym_2)));
}
