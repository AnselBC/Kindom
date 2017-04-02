//
// Created by 宋辰伟 on 2017/4/2.
//

#ifndef KINDOM_KING_ERROR_H
#define KINDOM_KING_ERROR_H



#include <stdarg.h>
#include "king/king_platform.h"
#include "king/king_apidefs.h"

void king_abort(const char *message_format, ...) KPRINTFLIKE(1, 2) KNORETURN;


#endif //KINDOM_KING_ERROR_H
