#include "section_identification.h"

char section_associated_symbols(uint16_t st_shndx, uint32_t sh_types, uint64_t sh_flags)
{
  char  c;

  (void)st_shndex;
  if (sh_types == STH_NULL)
    c = '?';
  else if (sh_types == SHT_NOTE)
    c = 'T'
}
