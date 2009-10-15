#include "Timer.h"
#include "TimerImpl.h"

namespace System
{

  Timer::Timer(Common::ICallbackPtr callback, unsigned period)
    : Callback(callback), Impl(new TimerImpl(Callback.Get(), period))
  {
  }

  Timer::~Timer()
  {
    delete Impl;
  }

}
