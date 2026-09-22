#include "../includes/include.h"

Elf64_Shdr *elf_sheader_64(Elf64_Ehdr *hdr, off_t size, bool swap) {

    uint64_t e_shoff = SWAP64(hdr->e_shoff, swap);
    uint16_t e_shentsize = SWAP16(hdr->e_shentsize, swap);
    uint16_t e_shnum = SWAP16(hdr->e_shnum, swap);

	if (e_shoff == 0 || e_shoff > (uint64_t)size || 
        (uint64_t)(e_shentsize * e_shnum) > (uint64_t)size - e_shoff) {
    return NULL;
}
	return (Elf64_Shdr *)((uint8_t *)hdr + e_shoff);
}

unsigned char get_nm_char_64(Elf64_Sym sym, Elf64_Shdr *sections, char *shstrtab, int shnum, bool swap) {

    unsigned char c = '?';

    uint16_t st_shndx = SWAP16(sym.st_shndx, swap);

    if (ELF64_ST_BIND(sym.st_info) == STB_WEAK) {
        c = 'W'; 
        if (st_shndx == SHN_UNDEF) 
            c = 'w';
        if (ELF64_ST_TYPE(sym.st_info) == STT_OBJECT) {
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
        
        Elf64_Shdr section = sections[st_shndx];

        uint32_t sh_type = SWAP32(section.sh_type, swap);
        uint64_t sh_flags = SWAP64(section.sh_flags, swap);
        uint32_t sh_name = SWAP32(section.sh_name, swap);

        if (sh_type == SHT_NOBITS)
            c = 'B';
        else if (!(sh_flags & SHF_ALLOC))
            c = 'N';
        else if (sh_flags & SHF_EXECINSTR)
            c = 'T';
        else if ((sh_flags & SHF_ALLOC) && !(sh_flags & SHF_WRITE))
            c = 'R';
        else
            c = 'D';
    }

    if (ELF64_ST_BIND(sym.st_info) == STB_LOCAL && c != '?') {
        c += 32;
    }

    return c;
}

void print_symbols_64(t_nm_sym *sort_array, int output_size, bool swap, t_nm_args *nm_args) {

    if (output_size == 0) {
        printf("No symbols\n");
    }

    if (!nm_args->r && !nm_args->p)
        qsort(sort_array, output_size, sizeof(t_nm_sym), compare_symbols);
    if (nm_args->r && !nm_args->p)
        qsort(sort_array, output_size, sizeof(t_nm_sym), rev_compare_symbols);

	for (int i = 0; i < output_size; i++) {
        
        Elf64_Sym *current_sym = sort_array[i].sym;
        char *current_name = sort_array[i].name;

        uint16_t st_shndx = SWAP16(current_sym->st_shndx, swap);
        uint64_t st_value = SWAP64(current_sym->st_value, swap);


        bool is_undefined = (strchr("vwuU", sort_array[i].c) != NULL);
        bool is_global = (strchr("vwVWUTACBRD", sort_array[i].c) != NULL);

        if (!is_undefined && nm_args->u)
            continue ;
        if (!is_global && nm_args->g)
            continue ;
        if (st_shndx == SHN_UNDEF) {
            printf("                 ");
        } 
		else {
            printf("%016lx ", st_value);
        }

        printf("%c %s\n", sort_array[i].c, current_name);
    }

}

int get_symbols_64(Elf64_Ehdr *hdr, Elf64_Shdr *symtab, Elf64_Shdr *sections, char *strtab, char *shstrtab, bool swap, off_t size, t_spec *spec, uint64_t strtab_size, t_nm_args *nm_args) {

    uint64_t sh_size = SWAP64(symtab->sh_size, swap);
    uint64_t sh_entsize = SWAP64(symtab->sh_entsize, swap);
    uint64_t sh_offset = SWAP64(symtab->sh_offset, swap);
    uint16_t e_shnum = SWAP16(hdr->e_shnum, swap);

    if (sh_entsize == 0) {
        print_error("ft_nm: error, division by zero\n");
        return (1);
    }

	int sym_count = sh_size / sh_entsize;
    
    if (sh_offset > size || sh_size > size - sh_offset) {
        nm_error(spec->filename, "has a section extending past end of file", "warning: ");
        return (1);
    }
	Elf64_Sym *symbols = (Elf64_Sym *)((uint8_t *)hdr + sh_offset);

	t_nm_sym *sort_array = malloc(sizeof(t_nm_sym) * sym_count);
    if (!sort_array) {
        print_error("ft_nm: malloc error\n");
        return (1);
    }

	int output_size = 0;

	for (int i = 1; i < sym_count; i++) {
        
        uint32_t st_name = SWAP32(symbols[i].st_name, swap);

        if (st_name >= strtab_size) {

            nm_error(spec->filename, "has a section extending past end of file", "warning: ");
            free(sort_array);
            return (1);
        }
        char *name = strtab + st_name;
        unsigned char type = ELF64_ST_TYPE(symbols[i].st_info);
        uint16_t st_shndx = SWAP16(symbols[i].st_shndx, swap);

        if (type == STT_SECTION && name[0] == '\0') {
            if (st_shndx < e_shnum) {
                uint32_t sh_name = SWAP32(sections[st_shndx].sh_name, swap);
                name = shstrtab + sh_name;
            }
        }

        bool is_file = (type == STT_FILE);
        bool is_section = (type == STT_SECTION);
        
        bool should_keep = false;

        if (nm_args->a) {
            should_keep = true;
        } else {
            should_keep = (name[0] != '\0' && !is_file && !is_section);
        }

        if (should_keep) {
            sort_array[output_size].sym = &symbols[i];
            sort_array[output_size].name = name;
            sort_array[output_size].value = SWAP64(symbols[i].st_value, swap);
            sort_array[output_size].c = get_nm_char_64(symbols[i], sections, shstrtab, e_shnum, swap);
            output_size++;
        }
    }
	
    print_symbols_64(sort_array, output_size, swap, nm_args);
    free(sort_array);
    return(0);
}

int core_engine_64(char *addr, off_t size, t_spec *spec, t_nm_args *nm_args, bool swap) {

	Elf64_Ehdr *hdr = (Elf64_Ehdr *)addr;

    Elf64_Shdr *sections = elf_sheader_64(hdr, size, swap);
    if (!sections) {
        nm_error(spec->filename, "file format not recognized", 0);
        return (1);
    }

    char *shstrtab = NULL;
    uint16_t e_shstrndx = SWAP16(hdr->e_shstrndx, swap);

    if (e_shstrndx != SHN_UNDEF) {
        Elf64_Shdr *shstrtab_sec = &sections[e_shstrndx];
        uint64_t sh_offset = SWAP64(shstrtab_sec->sh_offset, swap);
        shstrtab = (char *)((uint8_t *)hdr + sh_offset);
    }

    Elf64_Shdr *symtab = NULL;
    char *strtab = NULL;
    uint16_t e_shnum = SWAP16(hdr->e_shnum, swap);

    uint64_t str_size = 0;

    for (int i = 0; i < e_shnum; i++) {
        
        Elf64_Shdr *section = &sections[i];
        uint32_t sh_type = SWAP32(section->sh_type, swap);
        
        if (sh_type == SHT_SYMTAB) {
            symtab = section;
            uint32_t sh_link = SWAP32(symtab->sh_link, swap);
            if (sh_link >= e_shnum) {
                nm_error(spec->filename, "invalid section offset", 0);
                return 1;
            }
            Elf64_Shdr *strtab_section = &sections[sh_link];
            
            uint64_t str_offset = SWAP64(strtab_section->sh_offset, swap);
            str_size = SWAP64(strtab_section->sh_size, swap);

            if (strtab_section && str_offset + str_size <= (uint64_t)size) {
                strtab = (char *)((uint8_t *)hdr + str_offset);
            }
            else {
                nm_error(spec->filename, "Offset of table is exceeding file size", 0);
                return 1;
            }
            break;
        }
    }
    if (!symtab || !strtab || !shstrtab) {
        printf("ft_nm: %s: no symbols\n", spec->filename);
        return (1);
    }

	if (get_symbols_64(hdr, symtab, sections, strtab, shstrtab, swap, size, spec, str_size, nm_args)) {
        nm_error(spec->filename, "no symbols", 0);
        return (1);
    }
    return (0);
}
