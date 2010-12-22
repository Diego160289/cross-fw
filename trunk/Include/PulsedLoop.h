//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


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
    PulsedLoop(Common::ICallbackPtr callback, unsigned period);
    ~PulsedLoop();
  private:
    class PulsedLoopImpl;
    PulsedLoopImpl *Impl;
  };

}

#endif  // !__PULSEDLOOP_H__
