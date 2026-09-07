#include "../includes/include.h"

Elf64_Shdr *elf_sheader(Elf64_Ehdr *hdr, off_t size) {

	if (hdr->e_shoff == 0 ||
		hdr->e_shoff + (hdr->e_shentsize * hdr->e_shnum) > (uint64_t)size) {
			return NULL;
		}
	return (Elf64_Shdr *)((uint8_t *)hdr + hdr->e_shoff);
}

Elf64_Shdr *elf_section(Elf64_Ehdr *hdr, int idx, off_t size) {

	if (idx < 0 || idx >= hdr->e_shnum)
		return NULL;
	
	Elf64_Shdr *sections = elf_sheader(hdr, size);
	if (!sections)
		return NULL;

	return &sections[idx];
}

char *elf_shstr_table(Elf64_Ehdr *hdr, off_t size) {

	if (hdr->e_shstrndx == SHN_UNDEF)
		return NULL;
	
	Elf64_Shdr *str_section = elf_section(hdr, hdr->e_shstrndx, size);
	if (!str_section)
		return (NULL);
	
	if (str_section->sh_offset + str_section->sh_size > (uint64_t)size)
		return NULL;
	return (char *)((uint8_t *)hdr + str_section->sh_offset);
}


void core_engine(char *addr, int fd, off_t size) {

	Elf64_Ehdr *hdr = (Elf64_Ehdr *)addr;

	Elf64_Shdr *symtab = NULL;
    char *strtab = NULL;

	for (int i = 0; i < hdr->e_shnum; i++) {
		Elf64_Shdr *section = elf_section(hdr, i, size);
		if (!section) continue;

		if (section->sh_type == SHT_SYMTAB) {
			symtab = section;

			Elf64_Shdr *strtab_section = elf_section(hdr, symtab->sh_link, size);
			if (strtab_section) {
				if (strtab_section->sh_offset + strtab_section->sh_size > (uint64_t)size)
					return ;
				strtab = (char *)((uint8_t *)hdr + strtab_section->sh_offset);
			}
			break;
		}
	}

	if (!symtab || !strtab) {
		printf("No symbol table found\n");
       	return; // PRINT ERROR
    }

	if (symtab->sh_offset + symtab->sh_size > (uint64_t)size) {
		return ;
	}
	int sym_count = symtab->sh_size / symtab->sh_entsize;

	Elf64_Sym *symbols = (Elf64_Sym *)((uint8_t *)hdr + symtab->sh_offset);

	for (int i = 0; i < sym_count; i++) {
		
		char *name = strtab + symbols[i].st_name;

		if (name[0] != '\0') {
			printf("%016lx ? %s\n", symbols[i].st_value, name);		}
	}
}