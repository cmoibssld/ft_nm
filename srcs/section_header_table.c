#include <elf.h>
#include <inttypes.h> // For the PRIx64 macro...
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "endian.h"
#include "identification.h"
#include "main.h"
#include "output_formatting.h"
#include "section_header_info.h"
#include "section_header_table.h"
#include "symbols_sorted_array.h"

// Elf32_Word and Elf64_Word is the same: uint16_t. GNE !


// From what I have understand on the Section header table.
// Indeed the section header table is an ELF header where are store as a table all the headers on the sections. They contain information on the section such as names, sizes, locations, etc.,.
// The first information store, sh_name, aka the name of the function does not point to a string. It is not a name. It is an offset of a string in ... the section name string table. This section name string table ahas it's index define inside the ELF header: e_shstrndx.
// The location of the section in the ELF file image is given bt the section header variable: sh_offset.
// Name are interesting but the first thing to check for each section in the table is the sh_type. It will tell us wether a section is about.. A symbol : SHT_SYMTAB ! 

 const char *  get_string_table(const char* loaded_file, const void * section_header, const t_spec *specs, const t_section_table_data *info)
{
  const void * sh_table;

  sh_table = loaded_file + info->address;
  if (specs->arch == X32_BIT)
  {
    if (specs->e == LITTLE)
    {
      return (loaded_file + (((Elf32_Shdr *)(sh_table))[((Elf32_Shdr *)(section_header))->sh_link]).sh_offset);
    }
    else
      return (loaded_file + endian_swap32((((Elf32_Shdr *)(sh_table))[((Elf32_Shdr *)(section_header))->sh_link]).sh_offset));
  }
  else
  {
    if (specs->e == LITTLE)
    {
      return (loaded_file + (((Elf64_Shdr *)(sh_table))[((Elf64_Shdr *)(section_header))->sh_link]).sh_offset);
    }
    else
      return (loaded_file + endian_swap64((((Elf64_Shdr *)(sh_table))[((Elf64_Shdr *)(section_header))->sh_link]).sh_offset));
  }
}

t_section_table_status  symbol_table_id(const uint16_t st_info, const bool little_endian, const bool x32, const void *symbol_header, const char *string_table)
{
  uint8_t  type;
  uint8_t  binding;

  // (void)string_table;
  if (x32 == true)
  {
    type = ELF32_ST_TYPE(st_info);
    binding = ELF32_ST_BIND(st_info);
  }
  else
  {
    type = ELF64_ST_TYPE(st_info);
    binding = ELF64_ST_BIND(st_info);
  }
  // if (type == STT_NOTYPE) // one byte, endianness doesn't matter
  //   // print_undefined_symbol(binding, little_endian, x32, symbol_header, string_table);
  //   printf("No specific type\n");
  // else if (type == STT_OBJECT)
  //   printf("Variables, array, etc. found !\n");
  if (type == STT_FUNC) // only used now    
    print_function_symbol(binding, little_endian, x32, symbol_header, string_table);
  // else if (type == STT_SECTION)
  //   printf("Symbol + section. What is this?\n");
  // else if (type == STT_FILE)
  //   printf("A file name ! Nice\n");
  // else if (type == STT_COMMON)
  //   printf("Common data object\n");
  // else if (type == STT_TLS)
  //   printf("Thread local data object okk...\n");
  // else
  //   printf("Symbol found\n");
  return (CORRECT);
}

t_section_table_status  read_as_64bit(const char *loaded_file, const size_t loaded_size, const bool little_endian, const t_section_table_data *info)
{
  const Elf64_Shdr  *section_header;
  const char        *strtab;
  const Elf64_Sym   *symbol_header;
  uint16_t          i; // index in section header table
  uint16_t          j; // index in symbol table
  Elf64_Word        type;

 // alloc done to have a table of which symbol to look for. Then the loop after is only on that table

  section_header = (Elf64_Shdr *)(loaded_file + info->address);
  i = 0;
  while (i < info->total_entry)
  {
    if (info->address + i * sizeof(Elf64_Shdr) > loaded_size)
      return (SIZE_ERROR);
    type = section_header[i].sh_type;
    if (!little_endian)
      type = endian_swap32(type);
    if (type == SHT_SYMTAB) // || type == SHT_DYNSYM) // SHT_DYNSM is an option actually... aka a BONUS
    {
      if (!little_endian)
        strtab = loaded_file + endian_swap64((&section_header[endian_swap32(section_header[i].sh_link)])->sh_offset);
      else
        strtab = loaded_file + (&section_header[section_header[i].sh_link])->sh_offset;
      // printf("I found a symbol table! There is this much %d bytes in it\n", (uint16_t)section_header[i].sh_size);
      j = 1; // fist one of the table is all 0. Maybe should I check it ?
      symbol_header = (Elf64_Sym *)(loaded_file + section_header[i].sh_offset);
      while (j * sizeof(Elf64_Sym) < section_header[i].sh_size) // counting the number of symbols (indirect)
      {
        if (section_header[i].sh_offset + j * sizeof(Elf64_Sym) > loaded_size)
          return (SIZE_ERROR);
        if (symbol_header[j].st_shndx != SHN_UNDEF) // this is 0 so no problem with endian ?
          symbol_table_id(symbol_header[j].st_info,
                 little_endian, false, &symbol_header[j], strtab);
        else if (ELF64_ST_BIND(symbol_header[j].st_info) == STB_GLOBAL)
          print_undefined_symbol(little_endian, false, &symbol_header[j], strtab);
        ++j;
      }
    }
    i++;
  }
  return (CORRECT);
}

t_section_table_status  read_table(const char *restrict loaded_file, const size_t loaded_size, const t_spec *specs, const t_section_table_data *info, const t_options *opt)
// Note: t_section_table_data information are on litlle endiant coded. So no biggy to compare them with anything not from the file !
{
  s_symbol  *symbols_array;
  size_t    total_symbols;

  symbols_array = create_array(loaded_file, loaded_size, specs, info, &total_symbols);
  if (symbols_array == NULL)
    return(MEMORY_ALLOC_ERROR);
  sort_array(symbols_array, specs, total_symbols, opt);
  // print_array_important_stuff(symbols_array, specs, total_symbols);
  // must RETURN CORRECT
  free(symbols_array);
  // if (specs->arch == X32_BIT)
  //   return (read_as_32bit(loaded_file, loaded_size, specs->e == LITTLE, info));
  if (specs->arch == X64_BIT)
    return (read_as_64bit(loaded_file, loaded_size, specs->e == LITTLE, info));
  return (TABLE_INCOMPLETE);
}
