#ifndef MAIN_H
# define MAIN_H

# include <sys/stat.h> // struct stat and fstat function

char  *close_file(char *loaded_file, struct stat *statbuf, int *fd);
char  *open_file(const char *filename, struct stat *statbuf, int *fd); // fd is set to file fd

#endif
