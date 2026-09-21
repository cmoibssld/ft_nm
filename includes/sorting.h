#ifndef SORTING_H
#define SORTING_H

#include <elf.h>

int sym_compare32(const void *sym_1, const void *sym_2);
int sym_compare64(const void *sym_1, const void *sym_2);

int rev_sym_cmp32(const void *sym_1, const void *sym_2);
int rev_sym_cmp64(const void *sym_1, const void *sym_2);

#endif
