//
// Created by 宋辰伟 on 2017/4/2.
//

#include "king/king_platform.h"
#include "king/king_assert.h"
#include "king/king_error.h"
#include "king/king_string.h"

void
_king_assert(const char *expression, const char *file, int line)
{
    king_abort("%s:%d: failed assertion `%s`", file, line, expression);
}