//
// Created by 宋辰伟 on 2017/4/2.
//

#ifndef KINDOM_KING_ASSERT_H
#define KINDOM_KING_ASSERT_H

#include "king_error.h"
#include "king_apidefs.h"

#define kassert(EX) ((void)(likely(EX) ? (void)0 : _king_assert(#EX, __FILE__, __LINE__)))

#endif // KINDOM_KING_ASSERT_H
