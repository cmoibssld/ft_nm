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
	if(addr[EI_VERSION] != EV_CURRENT) {
		putstr("Not a valid object.\n", 2);
        return false;
	}
    return true;
}

t_elf_arch elf_check_architecture(unsigned char *addr) {

    if (!addr)
        return (INVALID_ARCH);

    if (addr[EI_CLASS] == ELFCLASS32) {
        return (X32_BIT);
        
    } else if (addr[EI_CLASS] == ELFCLASS64) {
        return (X64_BIT); 
        
    } else {
        return (INVALID_ARCH);
    }
}

t_elf_endian elf_check_endian(Elf32_Ehdr *hdr) {
	
    if(hdr->e_ident[EI_DATA] == ELFDATA2LSB) {
		return LITTLE;
	}
    else if (hdr->e_ident[EI_DATA] == ELFDATA2MSB) {
        return BIG;
    }
    else {
        return INVALID_ENDIAN;
    }
}

void	print_elf(t_spec *spec) {

	if (spec->arch == X64_BIT) {
		printf("64 BITS ARCHITECTURE\n");
	}
	else if (spec->arch == X32_BIT) {
		printf("32 BITS ARCHITECTURE\n");
	}

	if (spec->e == BIG) {
		printf("BIG ENDIAN\n");
	}
	else if (spec->e == LITTLE) {
		printf("LITTLE ENDIAN\n");
	}
}

int	check_elf(unsigned char *addr, int fd, t_spec *spec, off_t file_size) {

	if (file_size <= EI_NIDENT) {
        nm_error(spec->filename, "has a section extending past end of file", "warning: ");
        return (1);
    }
	// A RECHECK PEUT ETRE

	if (!elf_check_file(addr)) {
        nm_error(spec->filename, "file format not recognized", 0);
        return (1);
    }

    spec->e = elf_check_endian((Elf32_Ehdr *)addr);
    if (spec->e == INVALID_ARCH) {
        nm_error(spec->filename, "invalid endian architecture", 0);
        return (1);
    }

    spec->arch = elf_check_architecture(addr); 
    if (spec->arch == INVALID_ARCH) {
        nm_error(spec->filename, "architecture not supported", 0);
        return (1);
    }
	return (0);
	//print_elf(spec);
}