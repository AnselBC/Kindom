#include "kmemory.h"

int
main()
{
  char *a = (char *)kmalloc(10);
  kfree(a);
}
