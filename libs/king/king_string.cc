//
// Created by 宋辰伟 on 2017/4/2.
//

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "king/king_platform.h"
#include "king/king_assert.h"

size_t
kstrlcpy(char *dst, const char *src, size_t siz)
{
  char *d       = dst;
  const char *s = src;
  size_t n      = siz;

  /* Copy as many bytes as will fit */
  if (n != 0) {
    while (--n != 0) {
      if ((*d++ = *s++) == '\0')
        break;
    }
  }

  /* Not enough room in dst, add NUL and traverse rest of src */
  if (n == 0) {
    if (siz != 0)
      *d = '\0'; /* NUL-terminate dst */
    while (*s++)
      ;
  }

  return (s - src - 1); /* count does not include NUL */
}
