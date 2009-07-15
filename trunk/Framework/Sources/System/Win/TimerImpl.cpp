#include "PulsedTimerImpl.h"

namespace System
{

  PulsedTimer::PulsedTimerImpl::PulsedTimerImpl(Common::ICallback *callback, unsigned period, unsigned startAfter)
    : TimerQueue_(0)
    , Timer_(0)
  {
    if (!(TimerQueue_ = ::CreateTimerQueue()))
      throw PulsedTimerException("Can't create timer queue");
    if (!::CreateTimerQueueTimer(&Timer_, TimerQueue_, &PulsedTimerImpl::TimerFunc, callback, startAfter, period, 0))
    {
      ::DeleteTimerQueue(TimerQueue_);
      TimerQueue_ = NULL;
      throw PulsedTimerException("Can't create timer");
    }
  }

  PulsedTimer::PulsedTimerImpl::~PulsedTimerImpl()
  {
    ::DeleteTimerQueueTimer(TimerQueue_, Timer_, NULL);
    ::DeleteTimerQueue(TimerQueue_);
  }

  VOID CALLBACK PulsedTimer::PulsedTimerImpl::TimerFunc(PVOID param, BOOLEAN)
  {
    reinterpret_cast<Common::ICallback*>(param)->Do();
  }

}
