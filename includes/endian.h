#ifndef ENDIAN_H
# define ENDIAN_H

#include <stdatomic.h>
# include <stdint.h>

uint16_t  endian_swap16(const uint16_t n);
uint32_t  endian_swap32(const uint32_t n);
uint64_t  endian_swap64(const uint64_t n);

#endif
