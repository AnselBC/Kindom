#include <stdio.h>
#include <stdlib.h>

#include "P_EventSystem.h"

class Continuation;

class TestCont : public Continuation
{
public:
  int mainEvent(int event, void *data);
};

int
TestCont::mainEvent(int event, void *data)
{
  printf("handle Event\n");
  return 1;
}

int
main()
{
  std::shared_ptr<Mutex> m(new Mutex);
  TestCont *cont = new TestCont();
  cont->mutex    = m;
  SET_CONTINUATION_HANDLER(cont, &TestCont::mainEvent);
  Action *ac = new Action();
  *ac        = cont;
  ac->continuation->handleEvent(1, nullptr);
}
