//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __TIMER_H__
#define __TIMER_H__

#include "NoCopyable.h"
#include "Exceptions.h"
#include "SimpleIFaces.h"


namespace System
{

  DECLARE_RUNTIME_EXCEPTION(Timer)

  class Timer
    : private Common::NoCopyable
  {
  public:
    Timer(Common::ICallbackPtr callback, unsigned period);
    ~Timer();
  private:
    Common::ICallbackPtr Callback;
    class TimerImpl;
    TimerImpl *Impl;
  };

}

#endif  // !__TIMER_H__
