//
// Created by 宋辰伟 on 2017/4/2.
//
#include "king/king_platform.h"
#include "king/king_error.h"

static void
fatal_va(const char *hdr, const char *fmt, va_list ap)
{
    char msg[1024];
    const size_t len = strlen(hdr);

    strncpy(msg, hdr, sizeof(msg));
    vsnprintf(msg + len, sizeof(msg) - len, fmt, ap);
    msg[sizeof(msg) - 1] = 0;

    fprintf(stderr, "%s\n", msg);
    syslog(LOG_CRIT, "%s", msg);
}


void
king_abort(const char *message_format, ...)
{
    va_list ap;

    va_start(ap, message_format);
    fatal_va("Fatal: ", message_format, ap);
    va_end(ap);

    abort();
}
