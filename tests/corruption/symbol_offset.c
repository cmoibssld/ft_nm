#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <elf.h>
#include <stdbool.h>

#define SWAP16(val, swap) ((swap) ? swap16(val) : (val))
#define SWAP32(val, swap) ((swap) ? swap32(val) : (val))
#define SWAP64(val, swap) ((swap) ? swap64(val) : (val))

uint16_t swap16(uint16_t v) {
    return (v << 8) | (v >> 8);
}

uint32_t swap32(uint32_t v) {
    return ((v & 0x000000FFu) << 24) |
           ((v & 0x0000FF00u) << 8)  |
           ((v & 0x00FF0000u) >> 8)  |
           ((v & 0xFF000000u) >> 24);
}

uint64_t swap64(uint64_t v) {
    return ((v & 0x00000000000000FFull) << 56) |
           ((v & 0x000000000000FF00ull) << 40) |
           ((v & 0x0000000000FF0000ull) << 24) |
           ((v & 0x00000000FF000000ull) << 8)  |
           ((v & 0x000000FF00000000ull) >> 8)  |
           ((v & 0x0000FF0000000000ull) >> 24) |
           ((v & 0x00FF000000000000ull) >> 40) |
           ((v & 0xFF00000000000000ull) >> 56);
}

Elf64_Shdr *elf_sheader_64(Elf64_Ehdr *hdr, off_t size, bool swap) {

    uint64_t e_shoff = SWAP64(hdr->e_shoff, swap);
    uint16_t e_shentsize = SWAP16(hdr->e_shentsize, swap);
    uint16_t e_shnum = SWAP16(hdr->e_shnum, swap);

	if (e_shoff == 0 ||
		e_shoff + (e_shentsize * e_shnum) > (uint64_t)size) {
			return NULL;
		}
	return (Elf64_Shdr *)((uint8_t *)hdr + e_shoff);
}


void core_engine_64(char *addr, off_t size, bool swap) {

	Elf64_Ehdr *hdr = (Elf64_Ehdr *)addr;

    Elf64_Shdr *sections = elf_sheader_64(hdr, size, swap);
    if (!sections) return;

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

    for (int i = 0; i < e_shnum; i++) {
        
        Elf64_Shdr *section = &sections[i];
        uint32_t sh_type = SWAP32(section->sh_type, swap);
        
        if (sh_type == SHT_SYMTAB) {
            symtab = section;
            uint32_t sh_link = SWAP32(symtab->sh_link, swap);
            Elf64_Shdr *strtab_section = &sections[sh_link];
            
            uint64_t str_offset = SWAP64(strtab_section->sh_offset, swap);
            uint64_t str_size = SWAP64(strtab_section->sh_size, swap);

            if (strtab_section && str_offset + str_size <= (uint64_t)size) {
                strtab = (char *)((uint8_t *)hdr + str_offset);
            }
            break;
        }
    }
    if (!symtab || !strtab || !shstrtab) {
		perror("prout");
        return;
    }

	printf("offset is originally = %ul\n", (uint16_t)symtab->sh_offset);

	symtab->sh_offset = 10000;

	printf("offset is now = %ul\n", (uint16_t)symtab->sh_offset);
}

int main(int argc, char **argv) {
    int                 fd;
    struct stat         st;
    char                *addr;

    if (argc != 2)
        perror("Wrong numbers of arguments.");
    
    fd = open(argv[1], O_RDWR);
    if (fd < 0)
	{
        perror("Can't open file");
		return (1);
	}
    if (fstat(fd, &st) == -1) { /*GET SIZE FILE*/
        close(fd);
        perror("fstat");
		return (1);
    }
    if (!S_ISREG(st.st_mode)) {
        close(fd);
        perror("Not a regular file");
		return (1);
    }

    addr = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        close(fd);
        perror("mmap failed");
		return (1);
    }
	core_engine_64(addr, fd, st.st_size);
	msync(addr, st.st_size, MS_SYNC);
}
