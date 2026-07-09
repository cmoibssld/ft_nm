#include "../includes/include.h"



int main(int argc, char **argv) {

    int         fd;
    struct stat st;
    char     *addr;

    if (argc != 2)
        ft_error("Wrong numbers of arguments.\n", 1);
    
    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
        ft_error("Can't open file\n", 1);

    if (fstat(fd, &st) == -1) {  /* Pour obtenir la taille du fichier */
        close(fd);
        ft_error("fstat\n", 1);
    }

    addr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    
    if (addr == MAP_FAILED) {
        close(fd);
        ft_error("mmap failed\n", 1);
    }

    Elf64_Ehdr *elf_header = (Elf64_Ehdr *)addr;

    close(fd);
    return (0);
}