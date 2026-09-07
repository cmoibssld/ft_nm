#include "../includes/include.h"


// Elf32_Shdr *elf_sheader(Elf32_Ehdr *hdr) {
// 	return (Elf32_Shdr *)((int)hdr + hdr->e_shoff);
// }

// Elf32_Shdr *elf_section(Elf32_Ehdr *hdr, int idx) {
// 	return &elf_sheader(hdr)[idx];
// }

// char *elf_str_table(Elf32_Ehdr *hdr) {
// 	if(hdr->e_shstrndx == SHN_UNDEF) return NULL;
// 	return (char *)hdr + elf_section(hdr, hdr->e_shstrndx)->sh_offset;
// }

// char *elf_lookup_string(Elf32_Ehdr *hdr, int offset) {
// 	char *strtab = elf_str_table(hdr);
// 	if(strtab == NULL) return NULL;
// 	return strtab + offset;
// }

// int elf_32_function(Elf32_Ehdr *hdr, int fd) {

//     if (!elf_check_supported(hdr)) {
//         close(fd);
//         ft_error("ELF not supported\n", 1);
//     }

//     Elf32_Shdr *sheader = elf_sheader(hdr);
// }

int main(int argc, char **argv) {

    int                 fd;
    struct stat         st;
    unsigned char    *addr;
    t_spec            spec;

    if (argc != 2)
        ft_error("Wrong numbers of arguments.\n", 1);
    
    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
        ft_error("Can't open file\n", 1);

    if (fstat(fd, &st) == -1) { /*GET SIZE FILE*/
        close(fd);
        ft_error("fstat\n", 1);
    }
    if (!S_ISREG(st.st_mode)) {
        close(fd);
        ft_error("Not a regular file\n", 1);
    }

    addr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED) {
        close(fd);
        ft_error("mmap failed\n", 1);
    }

    check_elf(addr, fd, &spec, st.st_size);
    core_engine(addr, fd, st.st_size);
    
    close(fd);
    return (0);
}