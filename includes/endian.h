#ifndef ENDIAN_H
# define ENDIAN_H

# include <elf.h>
# include <stdatomic.h>
# include <stdint.h>

uint16_t    endian_swap16(const uint16_t n);
uint32_t    endian_swap32(const uint32_t n);
uint64_t    endian_swap64(const uint64_t n);

Elf32_Sym  *symbol_swap32(const Elf32_Sym *sym);
Elf64_Sym  *symbol_swap64(const Elf64_Sym *sym);

#endif
