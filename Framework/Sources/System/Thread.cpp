#include "Thread.h"
#include "ThreadImpl.h"

namespace System
{
  Thread::Thread(ThreadCallbackPtr callback)
    : Callback(callback)
    , Impl(new ThreadImpl(callback.Get()))
  {
  }

  Thread::~Thread()
  {
    delete Impl;
  }

}
