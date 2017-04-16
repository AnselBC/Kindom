#include "EventSystem.h"

static void * 
thread_main(void *)
{
	printf("thread start!!!\n");
}
int
main()
{
	Thread *thread1 = new Thread();
	Thread *thread2 = new Thread();
//	thread->set_specific();
//	printf("thread specified %p\n", thread->this_thread());
//	printf("get thread name: %s\n", thread->get_thread_name());
	thread1->start("[scw1]", 0, thread_main, nullptr, nullptr);	
	thread2->start("[scw2]", 0, thread_main, nullptr, nullptr); 
	sleep(1);
}
