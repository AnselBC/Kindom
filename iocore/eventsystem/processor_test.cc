#include "P_EventSystem.h"

class MC : public Continuation
{
public:
	int mainEvent(int event, void *data);
};

int
MC::mainEvent(int event, void *data)
{
	Thread *t = this_thread();
	printf("%p %ld\n", t, t->get_hrtime()/ HRTIME_SECOND);
	return 0;
}

int
main()
{
	MC *m = new MC();
	SET_CONTINUATION_HANDLER(m, &MC::mainEvent);
	EThread *eth = new EThread();
	eventProcessor.start(10, 1000000);
	eventProcessor.schedule_every(m, 1 * HRTIME_SECOND);

	while(true) {
		sleep(1);
	}
}
