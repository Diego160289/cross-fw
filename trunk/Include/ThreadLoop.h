//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __THREADLOOP_H__
#define __THREADLOOP_H__

#include "NoCopyable.h"
#include "SimpleIFaces.h"


namespace System
{
  class ThreadLoop
    : public Common::NoCopyable
  {
  public:
    ThreadLoop(Common::ICallbackPtr callback);
    ~ThreadLoop();
    void Resume();
  private:
    class ThreadLoopImpl;
    ThreadLoopImpl *Impl;
  };
}
#endif  // !__THREADLOOP_H__
