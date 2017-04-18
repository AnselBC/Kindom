//
// Created by 宋辰伟 on 2017/4/17.
//

#ifndef TEST_LOCK_KALIGN_H
#define TEST_LOCK_KALIGN_H

#define KMIN_ALIGN 8
#define KALIGN(size, boundary) (((size) + ((boundary)-1)) & ~((boundary)-1))

#endif // TEST_LOCK_KALIGN_H
