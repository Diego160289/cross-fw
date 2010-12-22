//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "ThreadImpl.h"


namespace System
{
  Thread::ThreadImpl::ThreadImpl(Common::ICallback *callback)
    : ThreadHandle(0)
  {
    if (pthread_create(&ThreadHandle, 0, &ThreadImpl::ThreadProc, callback))
      throw ThreadException("Can't create thread");
  }

  Thread::ThreadImpl::~ThreadImpl()
  {
    pthread_join(ThreadHandle, 0);
    pthread_detach(ThreadHandle);
  }

  void* Thread::ThreadImpl::ThreadProc(void *prm)
  {
    reinterpret_cast<Common::ICallback *>(prm)->Do();
    return 0;
  }

}
