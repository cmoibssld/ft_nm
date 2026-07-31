#include "errors.h"
#include "main.h"

int  main(int ac, char **av)
{
  if (ac > 1)
    return (print_error_message(av[1]));
  else
    return (print_error_message("no arg given"));
  // int  fd;
  // int  i;
  // char *ptr; // Memory where is store the file

  // fd = -1;
  // i = 1;
  // ptr = NULL;
  // while (i < ac)
  // {
    // ptr = open_file(av[i], &fd);
    // if (fd == -1 || ptr == NULL)
    // {
    //   return 0;
    // }
  // }
}
