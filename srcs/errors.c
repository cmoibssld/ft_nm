#include <errno.h> // errno, strerror
#include <stdio.h> // sprinf
#include <stdlib.h> // free

#include "errors.h"
#include "libft.h" // ft_calloc, ft_strlen

int  print_error_message(const char *problematic_arg)
{
  ssize_t  expected_bytes;
  char     *buff;
  char     *p_strerror;

  p_strerror = strerror(errno); // No free needed -> it's a string
  if (p_strerror == NULL || errno == EINVAL) // Both returns of strerror, depends of the system
    return (errno);
  expected_bytes = ft_strlen("nm: '': \n") + ft_strlen(problematic_arg) + ft_strlen(p_strerror);
  buff = (char *)ft_calloc(expected_bytes + 1, sizeof(char));
  if (buff == NULL)
    return (errno);
  sprintf(buff, "nm: '%s': %s\n", problematic_arg, strerror(errno));
  // only 2 bytes allocalted, good for me not too much memory consumption -> either this solution is less memory consumin but not as clear for reading: 
  // ft_strlcat(buff, "nm: ", ft_strlen("nm: "));
  // ft_strlcat(&buff[4], problematic_arg, ft_strlen(problematic_arg) + ft_strlen(buff) + 1);
  // ft_strlcat(buff, ": ", ft_strlen(": ") + ft_strlen(buff) + 1);
  // ft_strlcat(buff, p_strerror, ft_strlen(p_strerror) + ft_strlen(buff) + 1);
  write(STDERR_FILENO, buff, ft_strlen(buff));
  free(buff);
  return (0);
}
