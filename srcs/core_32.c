#include "../includes/include.h"

Elf32_Shdr *elf_sheader_32(Elf32_Ehdr *hdr, off_t size, bool swap) {

    uint32_t e_shoff = SWAP32(hdr->e_shoff, swap);
    uint16_t e_shentsize = SWAP16(hdr->e_shentsize, swap);
    uint16_t e_shnum = SWAP16(hdr->e_shnum, swap);

	if (e_shoff == 0 ||
		e_shoff + (e_shentsize * e_shnum) > (uint64_t)size) {
			return NULL;
		}
	return (Elf32_Shdr *)((uint8_t *)hdr + e_shoff);
}

unsigned char get_nm_char_32(Elf32_Sym sym, Elf32_Shdr *sections, char *shstrtab, int shnum, bool swap) {

    unsigned char c = '?';

    uint16_t st_shndx = SWAP16(sym.st_shndx, swap);

    if (ELF32_ST_BIND(sym.st_info) == STB_WEAK) {
        c = 'W'; 
        if (st_shndx == SHN_UNDEF) 
            c = 'w';
        if (ELF32_ST_TYPE(sym.st_info) == STT_OBJECT) {
			if (st_shndx == SHN_UNDEF)
				c = 'v';
			else
				c = 'V';
		}
    }
    else if (st_shndx == SHN_UNDEF) {
        c = 'U';
    }
    else if (st_shndx == SHN_ABS) {
        c = 'A';
    }
    else if (st_shndx == SHN_COMMON) {
        c = 'C';
    }

	else if (st_shndx < shnum) { 
        
        Elf32_Shdr section = sections[st_shndx];

        uint32_t sh_type = SWAP32(section.sh_type, swap);
        uint32_t sh_flags = SWAP32(section.sh_flags, swap);
        uint32_t sh_name = SWAP32(section.sh_name, swap);

        char *sec_name = shstrtab + sh_name;

        if (sh_type == SHT_NOBITS)
            c = 'B';
        else if (sh_flags & SHF_EXECINSTR)
            c = 'T';
        else if ((sh_flags & SHF_ALLOC) && !(sh_flags & SHF_WRITE))
            c = 'R';
        else
            c = 'D';
    }

    if (ELF32_ST_BIND(sym.st_info) == STB_LOCAL && c != '?') {
        c += 32;
    }

    return c;
}

void print_symbols_32(t_nm_sym *sort_array, int output_size, Elf32_Ehdr *hdr, Elf32_Shdr *sections, char *shstrtab, bool swap) {

    qsort(sort_array, output_size, sizeof(t_nm_sym), compare_symbols);

    uint16_t e_shnum = SWAP16(hdr->e_shnum, swap);

	for (int i = 0; i < output_size; i++) {
        
        Elf32_Sym *current_sym = sort_array[i].sym;
        char *current_name = sort_array[i].name;

        uint16_t st_shndx = SWAP16(current_sym->st_shndx, swap);
        uint32_t st_value = SWAP32(current_sym->st_value, swap);

        if (st_shndx == SHN_UNDEF) {
            printf("         ");
        } 
		else {
            printf("%08x ", st_value);
        }

        unsigned char c = get_nm_char_32(*current_sym, sections, shstrtab, e_shnum, swap);
        printf("%c %s\n", c, current_name);
    }

    free(sort_array);
}

void get_symbols_32(Elf32_Ehdr *hdr, Elf32_Shdr *symtab, Elf32_Shdr *sections, char *strtab, char *shstrtab, bool swap) {

    uint32_t sh_size = SWAP32(symtab->sh_size, swap);
    uint32_t sh_entsize = SWAP32(symtab->sh_entsize, swap);
    uint32_t sh_offset = SWAP32(symtab->sh_offset, swap);

	int sym_count = sh_size / sh_entsize;
	Elf32_Sym *symbols = (Elf32_Sym *)((uint8_t *)hdr + sh_offset);

	t_nm_sym *sort_array = malloc(sizeof(t_nm_sym) * sym_count);
    if (!sort_array)
        return;

	int output_size = 0;

	for (int i = 0; i < sym_count; i++) {
        
        uint32_t st_name = SWAP32(symbols[i].st_name, swap);

        char *name = strtab + st_name;
        unsigned char type = ELF32_ST_TYPE(symbols[i].st_info);

        if (name[0] != '\0' && type != STT_FILE) {
            sort_array[output_size].sym = &symbols[i];
            sort_array[output_size].name = name;
            sort_array[output_size].value = SWAP32(symbols[i].st_value, swap);
            output_size++;
        }
    }
	
    print_symbols_32(sort_array, output_size, hdr, sections, shstrtab, swap);
    return;
}

void core_engine_32(char *addr, int fd, off_t size, t_spec *spec, bool swap) {

	Elf32_Ehdr *hdr = (Elf32_Ehdr *)addr;

    Elf32_Shdr *sections = elf_sheader_32(hdr, size, swap);
    if (!sections) return;

    char *shstrtab = NULL;
    uint16_t e_shstrndx = SWAP16(hdr->e_shstrndx, swap);

    if (e_shstrndx != SHN_UNDEF) {
        Elf32_Shdr *shstrtab_sec = &sections[e_shstrndx];
        uint32_t sh_offset = SWAP32(shstrtab_sec->sh_offset, swap);
        shstrtab = (char *)((uint8_t *)hdr + sh_offset);
    }

    Elf32_Shdr *symtab = NULL;
    char *strtab = NULL;
    uint16_t e_shnum = SWAP16(hdr->e_shnum, swap);

    for (int i = 0; i < e_shnum; i++) {
        
        Elf32_Shdr *section = &sections[i];
        uint32_t sh_type = SWAP32(section->sh_type, swap);
        
        if (sh_type == SHT_SYMTAB) {
            symtab = section;
            uint32_t sh_link = SWAP32(symtab->sh_link, swap);
            Elf32_Shdr *strtab_section = &sections[sh_link];
            
            uint32_t str_offset = SWAP32(strtab_section->sh_offset, swap);
            uint32_t str_size = SWAP32(strtab_section->sh_size, swap);

            if (strtab_section && str_offset + str_size <= (uint64_t)size) {
                strtab = (char *)((uint8_t *)hdr + str_offset);
            }
            break;
        }
    }
    if (!symtab || !strtab || !shstrtab) {
        printf("ft_nm: %s: no symbols\n", spec->filename);
        return;
    }

	get_symbols_32(hdr, symtab, sections, strtab, shstrtab, swap);
    return ;
}