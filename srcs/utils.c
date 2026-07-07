#include "../includes/include.h"

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