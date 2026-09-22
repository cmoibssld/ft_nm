#include <unistd.h>

void  say_hello(void) {
  write(1, "Hello, world!\n", 14);
}
