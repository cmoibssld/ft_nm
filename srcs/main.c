#include "../includes/include.h"

bool elf_check_file(unsigned char *addr) {
    if(!addr) 
        return false;

    if(addr[EI_MAG0] != ELFMAG0) {
        putstr("ELF Header EI_MAG0 incorrect.\n", 2);
        return false;
    }

    if(addr[EI_MAG1] != ELFMAG1) {
        putstr("ELF Header EI_MAG1 incorrect.\n", 2);
        return false;
    }
    if(addr[EI_MAG2] != ELFMAG2) {
        putstr("ELF Header EI_MAG2 incorrect.\n", 2);
        return false;
    }
    if(addr[EI_MAG3] != ELFMAG3) {
        putstr("ELF Header EI_MAG3 incorrect.\n", 2);
        return false;
    }
    return true;
}

int elf_check_architecture(unsigned char *addr) {

    if (!addr)
        return (ARCHITECTURE_INVALID);

    if (addr[EI_CLASS] == ELFCLASS32) {
        return (ARCHITECTURE_32);
        
    } else if (addr[EI_CLASS] == ELFCLASS64) {
        return (ARCHITECTURE_64); 
        
    } else {
        return (ARCHITECTURE_INVALID);
    }
}

bool elf_check_supported(Elf32_Ehdr *hdr) {

	if(hdr->e_ident[EI_CLASS] != ELFCLASS32) {
		ERROR("Unsupported ELF File Class.\n");
		return false;
	}
	if(hdr->e_ident[EI_DATA] != ELFDATA2LSB) {
		ERROR("Unsupported ELF File byte order.\n");
		return false;
	}
	if(hdr->e_machine != EM_386) {
		ERROR("Unsupported ELF File target.\n");
		return false;
	}
	if(hdr->e_ident[EI_VERSION] != EV_CURRENT) {
		ERROR("Unsupported ELF File version.\n");
		return false;
	}
	if(hdr->e_type != ET_REL && hdr->e_type != ET_EXEC) {
		ERROR("Unsupported ELF File type.\n");
		return false;
	}
	return true;
}

Elf32_Shdr *elf_sheader(Elf32_Ehdr *hdr) {
	return (Elf32_Shdr *)((int)hdr + hdr->e_shoff);
}

Elf32_Shdr *elf_section(Elf32_Ehdr *hdr, int idx) {
	return &elf_sheader(hdr)[idx];
}

char *elf_str_table(Elf32_Ehdr *hdr) {
	if(hdr->e_shstrndx == SHN_UNDEF) return NULL;
	return (char *)hdr + elf_section(hdr, hdr->e_shstrndx)->sh_offset;
}

char *elf_lookup_string(Elf32_Ehdr *hdr, int offset) {
	char *strtab = elf_str_table(hdr);
	if(strtab == NULL) return NULL;
	return strtab + offset;
}

int elf_32_function(Elf32_Ehdr *hdr, int fd) {

    if (!elf_check_supported(hdr)) {
        close(fd);
        ft_error("ELF not supported\n", 1);
    }

    Elf32_Shdr *sheader = elf_sheader(hdr);
}

int main(int argc, char **argv) {

    int         fd;
    struct stat st;
    unsigned char     *addr;

    if (argc != 2)
        ft_error("Wrong numbers of arguments.\n", 1);
    
    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
        ft_error("Can't open file\n", 1);

    if (fstat(fd, &st) == -1) { /*GET SIZE FILE*/
        close(fd);
        ft_error("fstat\n", 1);
    }

    addr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    
    if (addr == MAP_FAILED) {
        close(fd);
        ft_error("mmap failed\n", 1);
    }

    if (!elf_check_file(addr)) {
        close(fd);
        ft_error("Not ELF file\n", 1);
    }

    int architecture_type = elf_check_architecture(addr); 

    if (architecture_type == ARCHITECTURE_64) {
        printf("64 BITS ARCHITECTURE\n");
        // Elf64_Ehdr *elf_header = (Elf64_Ehdr *)addr;
    }
    else if (architecture_type == ARCHITECTURE_32) {
        printf("32 BITS ARCHITECTURE\n");
        Elf32_Ehdr *elf_header = (Elf32_Ehdr *)addr;
        elf_32_function(elf_header, fd);
    }
    else {
        close(fd);
        ft_error("Architecture Error\n", 1);
    }
    
    close(fd);
    return (0);
}