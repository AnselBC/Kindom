//
// Created by 宋辰伟 on 2017/4/17.
//

#ifndef TEST_LOCK_I_CONTINUATION_H
#define TEST_LOCK_I_CONTINUATION_H

#include "I_Mutex.h"
#include "List.h"

#define CONTINUATION_EVENT_NONE 0

#define CONTINUATION_DONE 0
#define CONTINUATION_CONT 1

class Mutex;
class Continuation;

typedef int (Continuation::*ContinuationHandler)(int event, void *data);

class Continuation
{
public:
  ContinuationHandler handler;

#ifdef DEBUG
  const char *handler_name;
#endif

  std::shared_ptr<Mutex> mutex;

  int
  handleEvent(int event = CONTINUATION_EVENT_NONE, void *data = 0)
  {
    return (this->*handler)(event, data);
  }

  Continuation(Mutex *amutex = nullptr);
  Continuation(std::shared_ptr<Mutex> &amutex);
};

#ifdef DEBUG
#define SET_HANDLER(_h) (handler = ((ContinuationHandler)_h), handler_name = #_h)
#else
#define SET_HANDLER(_h) (handler = ((ContinuationHandler)_h))
#endif

#ifdef DEBUG
#define SET_CONTINUATION_HANDLER(_c, _h) (_c->handler = ((ContinuationHandler)_h), _c->handler_name = #_h)
#else
#define SET_CONTINUATION_HANDLER(_c, _h) (_c->handler = ((ContinuationHandler)_h))
#endif

inline Continuation::Continuation(std::shared_ptr<Mutex> &amutex)
  : handler(nullptr),
#ifdef DEBUG
    handler_name(nullptr),
#endif
    mutex(amutex)
{
}

inline Continuation::Continuation(Mutex *amutex)
  : handler(nullptr),
#ifdef DEBUG
    handler_name(nullptr),
#endif
    mutex(amutex)
{
}

#endif // TEST_LOCK_I_CONTINUATION_H
