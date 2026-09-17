#include <string.h>

#include "sorting.h"
#include "symbols_sorted_array.h"

int  sym_compare(const void *sym_1, const void *sym_2)
{
  const char *s1;
  const char *s2;

  s1 = ((const s_symbol *)sym_1)->name;
  s2 = ((const s_symbol *)sym_2)->name;
  
  return (strcmp(s1, s2));
}

int  rev_sym_cmp(const void *sym_1, const void *sym_2)
{
  return (-(sym_compare(sym_1, sym_2)));
}
