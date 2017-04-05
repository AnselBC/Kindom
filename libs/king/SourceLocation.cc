//
// Created by 宋辰伟 on 2017/4/5.
//

#include "SourceLocation.h"
#include "king_defs.h"
#include <stdio.h>
#include <string.h>


// This method takes a SourceLocation source location data structure and
// converts it to a human-readable representation, in the buffer <buf>
// with length <buflen>.  The buffer will always be NUL-terminated, and
// must always have a length of at least 1.  The buffer address is
// returned on success.  The routine will only fail if the SourceLocation is
// not valid, or the buflen is less than 1.

char *
SourceLocation::str(char *buf, int buflen) const
{
    const char *shortname;

    if (!this->valid() || buflen < 1)
        return (nullptr);

    shortname = strrchr(file, '/');
    shortname = shortname ? (shortname + 1) : file;

    if (func != nullptr) {
        snprintf(buf, buflen, "%s:%d (%s)", shortname, line, func);
    } else {
        snprintf(buf, buflen, "%s:%d", shortname, line);
    }
    buf[buflen - 1] = NUL;
    return (buf);
}