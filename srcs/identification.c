#include <elf.h> // ELF macro definitions
#include <stdbool.h>
#include <stdio.h> // printf

#include "errors.h" // error if not a ELF object file -> classic error handling
#include "identification.h"
#include "libft.h" // ft_bzero
#include "section_header_info.h" // next_step in core logic -> find header, find offset, find symbols ?
#include "section_header_table.h"

static const char *  get_strerror(const t_elf_id error)
// Each error when parsing ELF header shows differents message
{
  switch (error)
  {
    case NOT_A_ELF:
      return ("only ELF types accepted");
    case NOT_A_VALID_ARCH:
      return ("unsupported arch, only 32 or 64bits");
    case NOT_A_VALID_ENDIAN:
      return ("unrecognized endian");
    case NOT_A_VALID_VERSION:
      return ("ELF version is not current one");
    case NOT_A_VALID_OBJECT:
      return ("only x86_64, x86_32, object files and shared libraries can be given");
    default:
      return ("invalid format error");
  }
}

bool is_a_correct_elf_file(const char *restrict loaded_file, t_spec *specs)
// Check if it's an ELF file (magic number, the arch (32 or 64bits) and the byte order(endian))
{
  int      magic_number;

  magic_number = *(int *) loaded_file;
  if (magic_number != *(int *) ELFMAG)
    return (specs->status = NOT_A_ELF, false);

  specs->arch = arch_control(loaded_file);
  if (specs->arch == INVALID_ARCH)
    return (specs->status = NOT_A_VALID_ARCH, false);

  specs->e = endian_identification(loaded_file);
  if (specs->e == INVALID_ENDIAN)
    return (specs->status = NOT_A_VALID_ENDIAN, false);

  if (loaded_file[EI_VERSION] != EV_CURRENT)
    return (specs->status = NOT_A_VALID_OBJECT, false);

  if (specs->arch == X32_BIT)
    if (((Elf32_Ehdr *)loaded_file)->e_type == ET_NONE)
      return (specs->status = NOT_A_VALID_OBJECT, false);
  if (specs->arch == X64_BIT)
    if (((Elf64_Ehdr *)loaded_file)->e_type == ET_NONE)
      return (specs->status = NOT_A_VALID_OBJECT, false);
  
  return (specs->status = VALID_EFL_FILE, true);
}


t_elf_endian endian_identification(const char *restrict loaded_file)
{
  uint8_t  endian;

  endian = loaded_file[EI_DATA]; // Data encoding -> little-endian or big-endian
  if (endian == ELFDATA2LSB)
    return (LITTLE);
  else if (endian == ELFDATA2MSB)
    return (BIG);
  else
    return (INVALID_ENDIAN);
}

t_elf_arch  arch_control(const char * restrict loaded_file)
{
  uint8_t  arch;

  arch = loaded_file[EI_CLASS];
  if (arch == ELFCLASS32)
    return (X32_BIT);
  else if (arch == ELFCLASS64)
    return (X64_BIT);
  else
    return (INVALID_ARCH);
}

int  core_logic(const char * restrict param, const char * restrict loaded_file, size_t loaded_size, const t_options *opt)
{
  t_spec                 specs;
  t_section_table_data   info;

  ft_bzero(&specs, sizeof(t_spec));
  ft_bzero(&info, sizeof(t_section_table_data));
  if (loaded_size < EI_NIDENT || loaded_size + EI_NIDENT < sizeof(uint16_t))
    return(print_personnal_error(param, "file too small to be EFL"), 1);
  if (is_a_correct_elf_file(loaded_file, &specs) == false)
      return (print_personnal_error(param, get_strerror(specs.status)), 1);
  if (find_header_table_info(loaded_file, loaded_size, &specs, &info) == false)
      return (print_personnal_error(param, "Unexpected error while symbol processing"), 1);
  if (read_table(loaded_file, loaded_size, &specs, &info, opt) != CORRECT)
    return (print_personnal_error(param, "Error in section header table"), 1);
  return (0);
}
