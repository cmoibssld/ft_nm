#ifndef SECTION_IDENTIFICATION_H
# define SECTION_IDENTIFICATION_H

# include <stddef.h>
#include <sys/types.h>

char  section_associated_symbols(uint16_t st_shndx, uint32_t sh_types, uint64_t sh_flags);

#endif
