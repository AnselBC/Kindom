#include <stdio.h>
#include <stdlib.h>

#include "ts/ink_queue.h"
#include "P_Net.h"
#include "P_EventSystem.h"
#include "ProxyRaft.h"

static int stacksize = 1000000;

int
main(int /* argc ATS_UNUSED */, const char **argv)
{
	ink_freelist_init_ops(ink_freelist_malloc_ops());

	ink_event_system_init(makeModuleVersion(1, 0, PRIVATE_MODULE_HEADER));
	ink_net_init(makeModuleVersion(1, 0, PRIVATE_MODULE_HEADER));

  Thread *main_thread = new EThread;
  main_thread->set_specific();

	eventProcessor.start(2, stacksize);
	netProcessor.start(0, stacksize);

	ProxyPort::loadValue("8080:ipv4 8081:ipv6");
  init_ProxyServer(1);
  start_ProxyServer();

	while (true) {
		sleep(1);
	}
}
