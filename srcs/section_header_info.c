#include <elf.h>
#include <stdint.h>

#include "endian.h"
#include "identification.h"
#include "section_header_info.h"

// Symbols seems to be only in sections. Where to find sections ? Read the offset of the section header table -> (where it is). Inside it give the offset of the sections/the size 

// 1. Section header table is the e_shoff of the ELF Header. It's either Elf32_Off or Elf64_Off. So either an uint32_t or an uint64_t...

// Ok so I have the total number of section (great for my while loop) section header table entry count is elf32_half/elf64_half -> e_shnum. GNE: both uint16_t.
    // section header string table index is e_shstrndx
// good news everyone: Elf32_Half and Elf64_Half are the same: uint16_t

// commo section types. Dozens of types. The one interesting for us:
// .symtab -> Symbol table (names and addresses of function, variables ect.)

// To see if a section is a section for symbols look at the sh_name

// while loop should be a good idea for me

uint64_t get_section_header_offset(const char * restrict loaded_file, const size_t loaded_size, const t_spec * specs)
// get the section header table address into a uin64_t. Careful with endian.
{
  if ((specs->arch == X32_BIT && loaded_size < sizeof(Elf32_Ehdr)) ||
      (specs->arch == X64_BIT && loaded_size < sizeof(Elf64_Ehdr)))
    return (0);
  if (specs->e == LITTLE)
  {
    if (specs->arch == X32_BIT)
      return (((const Elf32_Ehdr *)loaded_file)->e_shoff);
    else if (specs->arch == X64_BIT)
      return (((const Elf64_Ehdr *)loaded_file)->e_shoff);
  }
  else if (specs->e == BIG)
  {
    if (specs->arch == X32_BIT)
      return (endian_swap32(((const Elf32_Ehdr *)loaded_file)->e_shoff));
    else if (specs->arch == X64_BIT)
      return (endian_swap64(((const Elf64_Ehdr *)loaded_file)->e_shoff));
  }
  return (0); // should trigger an error and an exit
}

uint16_t  get_section_header_entry_size(const char * restrict loaded_file,const size_t loaded_size, const t_spec * specs)
// size of one entry inside the section header table -> necessary of parsing
{
  if ((specs->arch == X32_BIT && loaded_size < sizeof(Elf32_Ehdr)) ||
      (specs->arch == X64_BIT && loaded_size < sizeof(Elf64_Ehdr)))
    return (0);
  if (specs->e == LITTLE)
  {
    if (specs->arch == X32_BIT)
      return (((const Elf32_Ehdr *)loaded_file)->e_shentsize);  
    else if (specs->arch == X64_BIT)
      return (((const Elf64_Ehdr *)loaded_file)->e_shentsize);
  }
  else if (specs->e == BIG)
  {
    if (specs->arch == X32_BIT)
      return (endian_swap16(((const Elf32_Ehdr *)loaded_file)->e_shentsize));
    else if (specs->arch == X64_BIT)
      return (endian_swap16(((const Elf64_Ehdr *)loaded_file)->e_shentsize));
  }
  return (0);
}

uint16_t  get_section_header_total_entry(const char *restrict loaded_file, const size_t loaded_size, const t_spec *specs)
{
  if ((specs->arch == X32_BIT && loaded_size < sizeof(Elf32_Ehdr)) ||
      (specs->arch == X64_BIT && loaded_size < sizeof(Elf64_Ehdr)))
    return (0);
  if (specs->e == LITTLE)
  {
    if (specs->arch == X32_BIT)
      return (((const Elf32_Ehdr *)loaded_file)->e_shnum);
    else if (specs->arch == X64_BIT)
      return (((const Elf64_Ehdr *)loaded_file)->e_shnum);
  }
  else if (specs->e == BIG)
  {
    if (specs->arch == X32_BIT)
      return (endian_swap16(((const Elf32_Ehdr *)loaded_file)->e_shnum));
    else if (specs->arch == X64_BIT)
      return (endian_swap16(((const Elf64_Ehdr *)loaded_file)->e_shnum));
  }
  return (0);
}

uint16_t  get_section_header_string_table_index(const char *restrict loaded_file, const size_t loaded_size, const t_spec *specs)
{
  if ((specs->arch == X32_BIT && loaded_size < sizeof(Elf32_Ehdr)) ||
      (specs->arch == X64_BIT && loaded_size < sizeof(Elf64_Ehdr)))
    return (0);
  if (specs->e == LITTLE)
  {
    if (specs->arch == X32_BIT)
      return (((const Elf32_Ehdr *)loaded_file)->e_shstrndx);
    else if (specs->arch == X64_BIT)
      return (((const Elf64_Ehdr *)loaded_file)->e_shstrndx);
  }
  else if (specs->e == BIG)
  {
    if (specs->arch == X32_BIT)
      return (endian_swap16(((const Elf32_Ehdr *)loaded_file)->e_shstrndx));
    else if (specs->arch == X64_BIT)
      return (endian_swap16(((const Elf64_Ehdr *)loaded_file)->e_shstrndx));
  }
  return (0);
}

bool  find_header_table_info(const char * restrict loaded_file, const size_t loaded_size, const t_spec * specs, t_section_table_data * info)
// looking at the whole ELF HEADER not just e_ident. So is the file big enough
{
  info->address = get_section_header_offset(loaded_file, loaded_size, specs);
  info->entry_size = get_section_header_entry_size(loaded_file,loaded_size, specs);
  info->total_entry = get_section_header_total_entry(loaded_file, loaded_size, specs);
  info->string_index = get_section_header_string_table_index(loaded_file, loaded_size, specs);
  
  if (!info->address || !info->entry_size || !info->total_entry || !info->string_index)
    return (false);
  // print_offset_res(info->address, info->entry_size, info->total_entry, info->string_index);
  return (true);
}

// for now I want to compare the result with readelf -h <param>
// 1. Do I get the same offset for header table
// 2. Is the size of the entries the same
// 3. Is the number of entries the same

#include <stdio.h>
void  print_offset_res(const uint16_t table_offset, const uint16_t section_entry_size, const uint16_t total_entry, const uint16_t string_index)
{
  printf("table header offset/address: %d\n", table_offset);
  printf("size of an entry inside the section header table: %d\n", section_entry_size);
  printf("total section table entry.ies: %d\n", total_entry);
  printf("section header string table index: %d\n", string_index);
}
