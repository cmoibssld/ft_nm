#ifndef MAIN_H
# define MAIN_H

# include <stdbool.h>
# include <sys/stat.h> // struct stat and fstat function

typedef struct {
  bool  a;
  bool  g;
  bool  u;
  bool  r;
  bool  p;
} t_options;

char  *close_file(char *loaded_file, struct stat *statbuf, int *fd);
char  *open_file(const char *filename, struct stat *statbuf, int *fd); // fd is set to file fd

bool  is_an_option(const char *param, t_options *opt);

#endif
