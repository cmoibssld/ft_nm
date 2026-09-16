#include "../includes/include.h"
#include <string.h>

Elf64_Shdr *elf_sheader_64(Elf64_Ehdr *hdr, off_t size) {

	if (hdr->e_shoff == 0 ||
		hdr->e_shoff + (hdr->e_shentsize * hdr->e_shnum) > (uint64_t)size) {
			return NULL;
		}
	return (Elf64_Shdr *)((uint8_t *)hdr + hdr->e_shoff);
}

unsigned char get_nm_char_64(Elf64_Sym sym, Elf64_Shdr *sections, char *shstrtab, int shnum) {

    unsigned char c = '?';

    if (ELF64_ST_BIND(sym.st_info) == STB_WEAK) {
        c = 'W'; 
        if (sym.st_shndx == SHN_UNDEF) 
            c = 'w';
        if (ELF64_ST_TYPE(sym.st_info) == STT_OBJECT) {
			if (sym.st_shndx == SHN_UNDEF)
				c = 'v';
			else
				c = 'V';
		}
    }
    else if (sym.st_shndx == SHN_UNDEF) {
        c = 'U';
    }
    else if (sym.st_shndx == SHN_ABS) {
        c = 'A';
    }
    else if (sym.st_shndx == SHN_COMMON) {
        c = 'C';
    }

	else if (sym.st_shndx < shnum) { 
        
        Elf64_Shdr section = sections[sym.st_shndx];
        char *sec_name = shstrtab + section.sh_name;

        if (section.sh_type == SHT_NOBITS || strcmp(sec_name, ".bss") == 0)
            c = 'B';
        else if (section.sh_flags & SHF_EXECINSTR || strcmp(sec_name, ".text") == 0)
            c = 'T';
        else if ((section.sh_flags & SHF_ALLOC) && !(section.sh_flags & SHF_WRITE))
            c = 'R';
        else
            c = 'D';
    }

    if (ELF64_ST_BIND(sym.st_info) == STB_LOCAL && c != '?') {
        c += 32;
    }

    return c;
}

void print_symbols_64(t_nm_sym *sort_array, int output_size, Elf64_Ehdr *hdr, Elf64_Shdr *sections, char *shstrtab) {

    qsort(sort_array, output_size, sizeof(t_nm_sym), compare_symbols);

	for (int i = 0; i < output_size; i++) {
        
        Elf64_Sym *current_sym = sort_array[i].sym;
        char *current_name = sort_array[i].name;

        if (current_sym->st_shndx == SHN_UNDEF) {
            printf("                 ");
        } 
		else {
            printf("%016lx ", current_sym->st_value);
        }

        unsigned char c = get_nm_char_64(*current_sym, sections, shstrtab, hdr->e_shnum);
        printf("%c %s\n", c, current_name);
    }

    free(sort_array);
}

void get_symbols_64(Elf64_Ehdr *hdr, Elf64_Shdr *symtab, Elf64_Shdr *sections, char *strtab, char *shstrtab) {

	int sym_count = symtab->sh_size / symtab->sh_entsize;
	Elf64_Sym *symbols = (Elf64_Sym *)((uint8_t *)hdr + symtab->sh_offset);

	t_nm_sym *sort_array = malloc(sizeof(t_nm_sym) * sym_count);
    if (!sort_array)
        return;

	int output_size = 0;

	for (int i = 0; i < sym_count; i++) {
        
        char *name = strtab + symbols[i].st_name;
        unsigned char type = ELF64_ST_TYPE(symbols[i].st_info);

        if (name[0] != '\0' && type != STT_FILE) {
            sort_array[output_size].sym = &symbols[i];
            sort_array[output_size].name = name;
            sort_array[output_size].value = symbols[i].st_value;
            output_size++;
        }
    }
	
    print_symbols_64(sort_array, output_size, hdr, sections, shstrtab);
    return;
}

void core_engine_64(char *addr, int fd, off_t size, t_spec *spec) {

	Elf64_Ehdr *hdr = (Elf64_Ehdr *)addr;

    Elf64_Shdr *sections = elf_sheader_64(hdr, size);
    if (!sections) return;

    char *shstrtab = NULL;
    if (hdr->e_shstrndx != SHN_UNDEF) {
        Elf64_Shdr *shstrtab_sec = &sections[hdr->e_shstrndx];
        shstrtab = (char *)((uint8_t *)hdr + shstrtab_sec->sh_offset);
    }

    Elf64_Shdr *symtab = NULL;
    char *strtab = NULL;

    for (int i = 0; i < hdr->e_shnum; i++) {
        Elf64_Shdr *section = &sections[i];

        if (section->sh_type == SHT_SYMTAB) {
            symtab = section;
            Elf64_Shdr *strtab_section = &sections[symtab->sh_link];
            
            if (strtab_section && strtab_section->sh_offset + strtab_section->sh_size <= (uint64_t)size) {
                strtab = (char *)((uint8_t *)hdr + strtab_section->sh_offset);
            }
            break;
        }
    }
    if (!symtab || !strtab || !shstrtab) {
        printf("ft_nm: %s: no symbols\n", spec->filename);
        return;
    }

	get_symbols_64(hdr, symtab, sections, strtab, shstrtab);
    return ;
}