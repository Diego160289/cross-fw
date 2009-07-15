#include "Timer.h"
#include "TimerImpl.h"

namespace System
{

  Timer::Timer(Common::ICallbackPtr callback, unsigned period, unsigned startAfter)
    : Callback(callback), Impl(new TimerImpl(Callback.Get(), period, startAfter))
  {
  }

  Timer::~Timer()
  {
    delete Impl;
  }

}
