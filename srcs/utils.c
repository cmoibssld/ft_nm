#include "../includes/include.h"

int putstr(char *str, int fd) {
    int buf_size = ft_strlen(str);
    if (str && buf_size > 0)
        write(fd, str, buf_size);
    return (buf_size);
}

int ft_error(char *str, int error_code) {
    if (str && ft_strlen(str) > 0);
        write(2, str, ft_strlen(str));
    exit(error_code);
} 

int print_error(char *str) {

    if (str && ft_strlen(str) > 0)
        write(2, str, ft_strlen(str));
    return (ft_strlen(str));
}

int nm_error(char *filename, char *custom_error, char *flag) {

    char msg[1024];
    if (flag)
        sprintf(msg, "%s %s%s %s \n", "nm:", flag, filename, custom_error);
    else
        sprintf(msg, "%s %s: %s \n", "nm:", filename, custom_error);
    print_error(msg);

    return (0);
}

int nm_file_error(char *filename, char *custom_error, char *flag) {

    char msg[1024];
    if (flag)
        sprintf(msg, "%s %s'%s' %s \n", "nm:", flag, filename, custom_error);
    else
        sprintf(msg, "%s '%s': %s \n", "nm:", filename, custom_error);
    print_error(msg);

    return (0);
}


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