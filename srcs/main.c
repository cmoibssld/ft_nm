#include "../includes/include.h"

int check_file(int fd, char *filename, t_spec *spec) {

    struct stat         st;
    unsigned char *addr = 0;

    if (fd < 0) {
        nm_file_error(filename, "No such file", 0);
        return (1);
    }

    if (fstat(fd, &st) == -1) { /*GET SIZE FILE*/
        nm_file_error(filename, "fstat error", 0);
        return (1);
    }
    if (!S_ISREG(st.st_mode)) {
        nm_file_error(filename, "is a directory", "Warning: ");
        return (1);
    }
    addr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED) {
        nm_error(filename, "malloc error", 0);
        return (1);
    }
    spec->addr = addr;
    spec->size = st.st_size;
    spec->filename = filename;

    return (0);
}

int do_nm (int fd, char *filename, t_spec *spec, bool print) {

    int exit_code = 0;

    if (check_file(fd, filename, spec))
        return (1);

    if (check_elf(spec->addr, fd, spec, spec->size))
        return (1);

    bool swap = 0;

    if (spec->e == BIG)
        swap = 1;

    if (print)
        printf("\n%s:\n", filename);
    
    if (spec->arch == X64_BIT)
        exit_code = core_engine_64(spec->addr, fd, spec->size, spec, swap);
    else if (spec->arch == X32_BIT)
        exit_code = core_engine_32(spec->addr, fd, spec->size, spec, swap);

    return (exit_code);
}

int main(int argc, char **argv) {

    int  exit_code = 0;
    char *filename = "a.out";

    if (argc <= 2) {
        t_spec  spec;

        if (argc == 2) 
            filename = argv[1];

        int fd = open(filename, O_RDONLY);
        do_nm(fd, filename, &spec, false);
        munmap(spec.addr, spec.size);
        close(fd);
        return (0);
    }

    for (int i = 1; i < argc; i++) {
    
            filename = argv[i];
            t_spec  spec;
            int code;

            int fd = open(filename, O_RDONLY);
            
            code = do_nm(fd, filename, &spec, true);
            
            if (exit_code == 0)
                exit_code = code;

            munmap(spec.addr, spec.size);
            close(fd);
    
        }
    return (exit_code);
}