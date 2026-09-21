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

int do_nm (int fd, char *filename, t_spec *spec, t_nm_args *nm_args, bool print) {

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
        exit_code = core_engine_64(spec->addr, spec->size, spec, nm_args, swap);
    else if (spec->arch == X32_BIT)
        exit_code = core_engine_32(spec->addr, spec->size, spec, nm_args, swap);

    return (exit_code);
}

int check_arg(char **argv, t_nm_args *nm_args) {

    nm_args->a = false;
    nm_args->g = false;
    nm_args->u = false;
    nm_args->r = false;
    nm_args->p = false;
    int i = 1;

    while (argv[i]) {

        int j = 0;

        while (argv[i][j]) {
         
            if (argv[i][0] != '-')
                return (i);
            while (argv[i][j] && argv[i][j] == '-')
               j++;
            while (argv[i][j] && (argv[i][j] == 'a' || argv[i][j] == 'g' || argv[i][j] == 'u' || argv[i][j] == 'r' || argv[i][j] == 'p')) {
                if (argv[i][j] == 'a')
                    nm_args->a = true;
                if (argv[i][j] == 'g')
                    nm_args->g = true;
                if (argv[i][j] == 'u')
                    nm_args->u = true;
                if (argv[i][j] == 'r')
                    nm_args->r = true;
                if (argv[i][j] == 'p')
                    nm_args->p = true;
                j++;
            }
            if (argv[i][j] && argv[i][j] != 'a' && argv[i][j] != 'g' && argv[i][j] != 'u' && argv[i][j] != 'r' && argv[i][j] != 'p') {
                print_error("nm: invalid options\n");
                return (-1);
            }
        }
        i++;
    }
    return (i);
}

int main(int argc, char **argv) {

    int  exit_code = 0;
    char *filename = "a.out";
    t_nm_args nm_args;
    bool print = true;

    int i = check_arg(argv, &nm_args);
    if (i < 0)
        return (1);

    if (argc - i == 1)
        print = false;
    
    if (argc <= 2) {
        t_spec  spec;

        if (argc == 2) 
            filename = argv[1];

        int fd = open(filename, O_RDONLY);
        do_nm(fd, filename, &spec, &nm_args, false);
        munmap(spec.addr, spec.size);
        close(fd);
        return (0);
    }

    for (i; i < argc; i++) {
    
            filename = argv[i];
            t_spec  spec;
            int code;

            int fd = open(filename, O_RDONLY);
            
            code = do_nm(fd, filename, &spec, &nm_args, print);
            
            if (exit_code == 0)
                exit_code = code;

            munmap(spec.addr, spec.size);
            close(fd);
    
        }
    return (exit_code);
}