#include "TimerImpl.h"

namespace System
{

  Timer::TimerImpl::TimerImpl(Common::ICallback *callback, unsigned period, unsigned startAfter)
    : TimerQueue_(0)
    , Timer_(0)
  {
    if (!(TimerQueue_ = ::CreateTimerQueue()))
      throw TimerException("Can't create timer queue");
    if (!::CreateTimerQueueTimer(&Timer_, TimerQueue_, &TimerImpl::TimerFunc, callback, startAfter, period, 0))
    {
      ::DeleteTimerQueue(TimerQueue_);
      TimerQueue_ = NULL;
      throw TimerException("Can't create timer");
    }
  }

  Timer::TimerImpl::~TimerImpl()
  {
    ::DeleteTimerQueueTimer(TimerQueue_, Timer_, NULL);
    ::DeleteTimerQueue(TimerQueue_);
  }

  VOID CALLBACK Timer::TimerImpl::TimerFunc(PVOID param, BOOLEAN)
  {
    reinterpret_cast<Common::ICallback*>(param)->Do();
  }

}
