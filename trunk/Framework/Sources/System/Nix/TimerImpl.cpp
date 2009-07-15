#include "PulsedTimerImpl.h"

#include <string.h>

namespace System
{

  PulsedTimer::PulsedTimerImpl::PulsedTimerImpl(Common::ICallback *callback, unsigned period, unsigned startAfter)
    : TimerId(0)
  {
    memset(&SigEvent, 0, sizeof(SigEvent));
    memset(&Value, 0, sizeof(Value));

    Value.it_value.tv_sec = period / 1000;
    Value.it_value.tv_nsec = (period % 1000) * 1000000;

    Value.it_interval.tv_sec = period / 1000;
    Value.it_interval.tv_nsec = (period % 1000) * 1000000;

    SigEvent.sigev_notify = SIGEV_THREAD;
    SigEvent.sigev_notify_function = &PulsedTimerImpl::TimerFunc;
    SigEvent.sigev_notify_attributes = NULL;
    SigEvent.sigev_value.sival_ptr = callback;

    if (timer_create(CLOCK_REALTIME, &SigEvent, &TimerId))
    {
      timer_delete(TimerId);
      throw PulsedTimerException("Can't create timer");
    }

    if (timer_settime(TimerId, 0, &Value, NULL))
      throw PulsedTimerException("Can't set timer");
  }

  PulsedTimer::PulsedTimerImpl::~PulsedTimerImpl()
  {
    timer_delete(TimerId);
  }

  void PulsedTimer::PulsedTimerImpl::TimerFunc(union sigval val)
  {
    reinterpret_cast<Common::ICallback*>(val.sival_ptr)->Do();
  }
}
