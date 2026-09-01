#include "endian.h"

uint16_t  endian_swap16(const uint16_t n)
{
  return (((0xFF00 & n) >> 8) | ((0x00FF & n) << 8)); 
}

uint32_t  endian_swap32(const uint32_t n)
{
  return (((0xFF000000 & n) >> 24) |
        ((0x00FF0000 & n) >> 8) |
        ((0x0000FF00 & n) << 8) |
        ((0x000000FF & n) << 24));
}

uint64_t  endian_swap64(const uint64_t n)
// ULL suffix is important so the hexadecimal are treated as 64bit unsigned value
{
  return (((0xFF00000000000000ULL & n) >> 56) |
          ((0x00FF000000000000ULL & n) >> 40) |
          ((0x0000FF0000000000ULL & n) >> 24) |
          ((0x000000FF00000000ULL & n) >> 8)  |
          ((0x00000000FF000000ULL & n) << 8)  |
          ((0x0000000000FF0000ULL & n) << 24) |
          ((0x000000000000FF00ULL & n) << 40) |
          ((0x00000000000000FFULL & n) << 56));
}

// #include <stdio.h>
// #include <inttypes.h>
// int main(void)
// {
//     uint16_t n16 = 0x1234;
//     uint32_t n32 = 0x12345678;
//     uint64_t n64 = 0x123456789ABCDEF0ULL;

//     printf("16-bit:\n");
//     printf("Input    : 0x%04" PRIX16 "\n", n16);
//     printf("Swapped  : 0x%04" PRIX16 "\n", endian_swap16(n16));
//     printf("Expected : 0x3412\n\n");

//     printf("32-bit:\n");
//     printf("Input    : 0x%08" PRIX32 "\n", n32);
//     printf("Swapped  : 0x%08" PRIX32 "\n", endian_swap32(n32));
//     printf("Expected : 0x78563412\n\n");

//     printf("64-bit:\n");
//     printf("Input    : 0x%016" PRIX64 "\n", n64);
//     printf("Swapped  : 0x%016" PRIX64 "\n", endian_swap64(n64));
//     printf("Expected : 0xF0DEBC9A78563412\n");

//     return 0;
// }
