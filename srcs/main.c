#include "main.h"

int  main(int ac, char **av)
{
  int  fd;
  uint32_t  i;
  char *ptr; // Memory where is store the file

  fd = -1;
  i = 1;
  ptr = NULL;
  while (i < ac)
  {
    ptr = open_file(ac[i], &fd);
    if (fd == -1 || ptr == NULL)
    {
      
    }
  }
}
