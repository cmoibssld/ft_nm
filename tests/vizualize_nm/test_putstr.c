#include <stdlib.h>
#include <unistd.h>

ssize_t  ft_putstr(const char *str);
ssize_t  ft_strlen(const char *str);

int  main(void)
{
  ssize_t i;
  
  i = ft_putstr("Hello world!\n");
  if (i != ft_strlen("Hello world!\n"))
  {
    return (EXIT_FAILURE);
  }
  else
  {
    return (EXIT_SUCCESS);
  }
}

ssize_t ft_putstr(const char *str)
{
  ssize_t i;
  
  if (str == NULL)
  {
    return (-1);
  }
  i = write(STDIN_FILENO, &str[0], ft_strlen(str));
  return (i);
}

ssize_t  ft_strlen(const char *str)
{
  ssize_t i;

  i = 0;
  if (str == NULL)
  {
    return (-1);
  }
  while (str[i] != '\0')
  {
    i++;
  }
  return (i);
}
