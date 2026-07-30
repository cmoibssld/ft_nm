#include <stdlib.h>
#include "errors.h"


int  print_error_message(cont char *problematic_arg)
{
  ssize_t  expected_bytes;
  ssize_t  writen_bytes;
  char     *buff;

  buff = (char *)calloc(strlen("nm: ") + 1, sizeof(char));
  if (buff == NULL)
    return (MEM_ERROR);

  free(buff);
  return (expected_bytes - written_bytes);
}
