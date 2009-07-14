#include "ThreadLoop.h"
#include "SyncObj.h"

namespace System
{
  ThreadLoop::ThreadLoop(Common::ICallbackPtr callback)
    : IsRun(true)
    , Callback(callback)
  {
    InternalCallback = Common::CreateMemberCakkback(*this, &ThreadLoop::CallbackFunc);
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
        if (IsRun)
          Event_.Wait();
        else
          break;
        if (IsRun)
          Callback->Do();
        else
          break;
        {
          Common::SyncObject<Mutex> Locker(Mutex_);
          if (IsRun)
            Event_.Reset();
          else
            break;
        }
      }
      catch (std::exception &)
      {
      }
    }
  }
}
