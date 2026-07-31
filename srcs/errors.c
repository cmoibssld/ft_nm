#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "errors.h"
#include "libft.h"

int  print_error_message(const char *problematic_arg)
{
  ssize_t  expected_bytes;
  char     *buff;

  expected_bytes = 6 + ft_strlen(problematic_arg) + ft_strlen(strerror(errno));
  buff = (char *)ft_calloc(expected_bytes + 1, sizeof(char));
  if (buff == NULL)
    return (errno);
  sprintf(buff, "nm: %s: %s", problematic_arg, strerror(errno));
  perror(buff);
  free(buff);
  return (0);
}
