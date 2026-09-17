#include "../includes/include.h"

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
    spec.filename = argv[1];

    check_elf(addr, fd, &spec, st.st_size);

    bool swap = 0;

    if (spec.e == BIG)
        swap = 1;

    if (spec.arch == X64_BIT)
        core_engine_64(addr, fd, st.st_size, &spec, swap);
    else if (spec.arch == X32_BIT)
        core_engine_32(addr, fd, st.st_size, &spec, swap);

    close(fd);
    return (0);
}