//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __THREAD_H__
#define __THREAD_H__

#include "NoCopyable.h"
#include "SimpleIFaces.h"
#include "Pointers.h"
#include "Exceptions.h"


namespace System
{
  DECLARE_RUNTIME_EXCEPTION(Thread)

  class Thread
    : private Common::NoCopyable
  {
  public:
    Thread(Common::ICallbackPtr callback);
    ~Thread();
  private:
    Common::ICallbackPtr Callback;
    class ThreadImpl;
    ThreadImpl *Impl;
  };
}

#endif  // !__THREAD_H__
