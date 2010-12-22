//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


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
