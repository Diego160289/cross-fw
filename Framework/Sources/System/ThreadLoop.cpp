#include "ThreadLoop.h"
#include "SyncObj.h"

namespace System
{
  ThreadLoop::ThreadLoop(Thread::ThreadCallbackPtr callback)
    : IsRun(true)
    , Callback(callback)
  {
    InternalCallback.Reset(new Common::IMemberCallbackImpl<ThreadLoop>(*this, &ThreadLoop::CallbackFunc));
    Event_.Reset();
    Thread_.Reset(new Thread(InternalCallback));
  }

  ThreadLoop::~ThreadLoop()
  {
    {
      Common::SyncObject<Mutex> Locker(Mutex_);
      IsRun = false;
      Event_.Set();
    }
    Thread_.Release();
  }

  void ThreadLoop::Resume()
  {
    Common::SyncObject<Mutex> Locker(Mutex_);
    Event_.Set();
  }

  void ThreadLoop::CallbackFunc()
  {
    while (true)
    {
      try
      {
        Event_.Wait();
        if (IsRun)
          Callback->Do();
        else
          break;
        {
          Common::SyncObject<Mutex> Locker(Mutex_);
          Event_.Reset();
        }
      }
      catch (std::exception &)
      {
      }
    }
  }
}
