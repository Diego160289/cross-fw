//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "PulsedLoop.h"
#include "Timer.h"
#include "ThreadLoop.h"
#include "Pointers.h"


namespace System
{

  class PulsedLoop::PulsedLoopImpl
    : private Common::NoCopyable
  {
  public:
    PulsedLoopImpl(Common::ICallbackPtr callback, unsigned period)
    {
      Loop.Reset(new ThreadLoop(callback));
      Timer_.Reset(new Timer(Common::CreateMemberCallback(*this, &PulsedLoopImpl::Resume), period));
    }
  private:
    Common::SharedPtr<ThreadLoop> Loop;
    Common::SharedPtr<Timer> Timer_;
    void Resume()
    {
      Loop->Resume();
    }
  };

  PulsedLoop::PulsedLoop(Common::ICallbackPtr callback, unsigned period)
    : Impl(new PulsedLoopImpl(callback, period))
  {
  }

  PulsedLoop::~PulsedLoop()
  {
    delete Impl;
  }

}
