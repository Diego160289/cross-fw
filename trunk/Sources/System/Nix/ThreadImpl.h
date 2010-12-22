//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __THREADIMPL_H__
#define __THREADIMPL_H__

#include "Thread.h"

#include <pthread.h>


namespace System
{
  class Thread::ThreadImpl
    : private Common::NoCopyable
  {
  public:
    ThreadImpl(Common::ICallback *callback);
    ~ThreadImpl();
  private:
    pthread_t ThreadHandle;
    static void* ThreadProc(void *prm);
  };

}

#endif  // !__THREADIMPL_H__
