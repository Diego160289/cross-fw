//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "Thread.h"
#include "ThreadImpl.h"


namespace System
{
  Thread::Thread(Common::ICallbackPtr callback)
    : Callback(callback)
    , Impl(new ThreadImpl(callback.Get()))
  {
  }

  Thread::~Thread()
  {
    delete Impl;
  }

}
