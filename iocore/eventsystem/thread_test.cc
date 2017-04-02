//
// Created by 宋辰伟 on 2017/4/2.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "I_EventSystem.h"

int
main()
{
    Thread *thread = new Thread();
    thread->start("hello", 0, nullptr, nullptr, nullptr);
	sleep(1);
}
