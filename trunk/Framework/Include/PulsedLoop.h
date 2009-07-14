#ifndef __PULSEDLOOP_H__
#define __PULSEDLOOP_H__

#include "NoCopyable.h"
#include "SimpleIFaces.h"

namespace System
{

  class PulsedLoop
    : Common::NoCopyable
  {
  public:
    PulsedLoop(Common::ICallbackPtr callback, unsigned period, unsigned startAfter);
    ~PulsedLoop();
  private:
    class PulsedLoopImpl;
    PulsedLoopImpl *Impl;
  };

}

#endif  // !__PULSEDLOOP_H__
