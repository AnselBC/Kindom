#include <stdio.h>
#include <stdlib.h>

#include "ts/ink_queue.h"
#include "P_Net.h"
#include "P_EventSystem.h"

static int stacksize = 1000000;

int
main(int /* argc ATS_UNUSED */, const char **argv)
{
	ink_freelist_init_ops(ink_freelist_malloc_ops());

	ink_event_system_init(makeModuleVersion(1, 0, PRIVATE_MODULE_HEADER));
	ink_net_init(makeModuleVersion(1, 0, PRIVATE_MODULE_HEADER));

	eventProcessor.start(1, stacksize);
	netProcessor.start(0, stacksize);

	while (true) {
		sleep(1);
	}
}
