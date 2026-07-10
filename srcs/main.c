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
        Elf64_Ehdr *elf_header = (Elf64_Ehdr *)addr;
    }
    else if (architecture_type == ARCHITECTURE_32) {
        printf("32 BITS ARCHITECTURE\n");
        Elf32_Ehdr *elf_header = (Elf32_Ehdr *)addr;
    }
    else {
        close(fd);
        ft_error("Architecture Error\n", 1);
    }
    
    close(fd);
    return (0);
}