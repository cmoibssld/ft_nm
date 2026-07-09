#include "../includes/include.h"

int putstr(char *str, int fd) {
    int buf_size = ft_strlen(str);
    write(fd, str, buf_size);
    return (buf_size);
}

int ft_strlen(char *str) {
    int i = 0;
    while (str && str[i]) {
        i++;
    }
    return (i);
}

int ft_error(char *str, int error_code) {
    write(2, str, ft_strlen(str));
    exit(error_code);
} 