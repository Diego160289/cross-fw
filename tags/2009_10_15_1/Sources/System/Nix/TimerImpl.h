#ifndef __TIMERIMPL_H__
#define __TIMERIMPL_H__

#include "Timer.h"

#include <sys/time.h>
#include <sys/signal.h>
#include <pthread.h>


namespace System
{

  class Timer::TimerImpl
    : private Common::NoCopyable
  {
  public:
    TimerImpl(Common::ICallback *callback, unsigned period);
    ~TimerImpl();
  private:
    sigevent SigEvent;
    timer_t TimerId;
    itimerspec Value;
    static void TimerFunc(union sigval val);
  };

}

#endif  // !__TIMERIMPL_H__
