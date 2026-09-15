#include <fcntl.h> // open
#include <stdbool.h>
#include <stdlib.h> // free
#include <sys/mman.h> // mmap and unmap
#include <unistd.h> // close, write

#include "errors.h" // print_error_message
#include "identification.h"
#include "libft.h" // ft_calloc
#include "main.h"

bool  is_an_option(const char *param, t_options *opt)
{
  if (param[0] != '-' || ft_strlen(param) != 2)
    return (false);
  switch (param[1]) {
    case 'a': {
        opt->a = true;
        return (true);
      }
    case 'g': {
        opt->g = true;
        return (true);
      }
    case 'u': {
        opt->u = true;
        return (true);
      }
    case 'r': {
        opt->r = true;
        return (true);
      }
    case 'p': {
        opt->p = true;
        return (true);
      }
    default: {
      return (false);
    }
  }
}

char  *close_file(char *loaded_file, struct stat *statbuf, int *fd)
{
  if (loaded_file != NULL && statbuf != NULL)
    munmap(loaded_file, statbuf->st_size);
  if (*fd != -1)
    close(*fd);
  return (NULL);
}

char  *open_file(const char *file_name, struct stat *statbuf, int *fd)
{
  char          *loaded_file;

  *fd = open(file_name, O_RDONLY);
  if (*fd == -1)
    return (NULL);
  if (fstat(*fd, statbuf) == -1)
    return (close_file(NULL, statbuf, fd));
  loaded_file = mmap(NULL, statbuf->st_size, PROT_READ, MAP_PRIVATE, *fd, 0);
  // subject says that one way to broke nm is whith invalid offset. Set offset here to 0 but maybe this is actually one of the problem ?
  if (loaded_file == NULL)
    return (close_file(NULL, statbuf, fd));
  return (loaded_file);
}

int  main(int ac, char **av)
{
  int          fd;
  int          i;
  int          return_code;
  char         *loaded_file; // Memory where is store the file
  struct stat  *statbuf;
  t_options    opt;
  
  return_code = 0;
  ft_bzero(&opt, sizeof(t_options));
  statbuf = ft_calloc(1, sizeof(struct stat));
  if (statbuf == NULL)
    return (print_error_message(av[0]));
  for (int i = 1; i < ac; ++i)
    is_an_option(av[i], &opt);
  i = 0;
  while (++i < ac)
  {
    if (is_an_option(av[i], &opt))
      continue ;
    fd = -1;
    loaded_file = NULL;
    loaded_file = open_file(av[i], statbuf, &fd);
    if (loaded_file == NULL)
    {
      print_error_message(av[i]);
      return_code = 1;
      continue ;
    }
    // printf("%X\n", *(int *)loaded_file); // print first 4 bytes -> magic number. Careful, little or big endian change way of reading the bytes !!
    // printf("%s\n", loaded_file);
    return_code += core_logic(av[i], loaded_file, statbuf->st_size, &opt);
    close_file(loaded_file, statbuf, &fd);
  }
  free(statbuf);
  return (return_code);
}

// int  main(int ac, char **av)
// {
//    if (ac > 1)
//     return (print_error_message(av[1]));
//   else
//     return (print_error_message("no arg given"));
// }
