//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "ThreadLoop.h"
#include "SyncObj.h"
#include "Pointers.h"
#include "Mutex.h"
#include "Thread.h"
#include "ManualEvent.h"


namespace System
{
  class ThreadLoop::ThreadLoopImpl
    : public Common::NoCopyable
  {
  public:
    ThreadLoopImpl(Common::ICallbackPtr callback);
    ~ThreadLoopImpl();
    void Resume();
  private:
    Common::ICallbackPtr InternalCallback;
    volatile bool IsRun;
    Common::ICallbackPtr Callback;
    Mutex Mutex_;
    ManualEvent Event_;
    Common::SharedPtr<Thread> Thread_;
    void CallbackFunc();
  };

  ThreadLoop::ThreadLoopImpl::ThreadLoopImpl(Common::ICallbackPtr callback)
    : IsRun(true)
    , Callback(callback)
  {
    InternalCallback = Common::CreateMemberCallback(*this, &ThreadLoop::ThreadLoopImpl::CallbackFunc);
    Event_.Reset();
    Thread_.Reset(new Thread(InternalCallback));
  }

  ThreadLoop::ThreadLoopImpl::~ThreadLoopImpl()
  {
    {
      Common::SyncObject<Mutex> Locker(Mutex_);
      IsRun = false;
      Event_.Set();
    }
    Thread_.Release();
  }

  void ThreadLoop::ThreadLoopImpl::Resume()
  {
    Common::SyncObject<Mutex> Locker(Mutex_);
    Event_.Set();
  }

  void ThreadLoop::ThreadLoopImpl::CallbackFunc()
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

  ThreadLoop::ThreadLoop(Common::ICallbackPtr callback)
    : Impl(new ThreadLoopImpl(callback))
  {
  }

  ThreadLoop::~ThreadLoop()
  {
    delete Impl;
  }

  void ThreadLoop::Resume()
  {
    Impl->Resume();
  }
}
