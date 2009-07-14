#include "PulsedLoop.h"
#include "PulsedTimer.h"
#include "ThreadLoop.h"
#include "Pointers.h"

namespace System
{

  class PulsedLoop::PulsedLoopImpl
    : private Common::NoCopyable
  {
  public:
    PulsedLoopImpl(Common::ICallbackPtr callback, unsigned period, unsigned startAfter)
    {
      Loop.Reset(new ThreadLoop(callback));
      Timer.Reset(new PulsedTimer(Common::CreateMemberCallback(*this, &PulsedLoopImpl::Resume), period, startAfter));
    }
  private:
    Common::SharedPtr<ThreadLoop> Loop;
    Common::SharedPtr<PulsedTimer> Timer;
    void Resume()
    {
      Loop->Resume();
    }
  };

  PulsedLoop::PulsedLoop(Common::ICallbackPtr callback, unsigned period, unsigned startAfter)
    : Impl(new PulsedLoopImpl(callback, period, startAfter))
  {
  }

  PulsedLoop::~PulsedLoop()
  {
    delete Impl;
  }

}
