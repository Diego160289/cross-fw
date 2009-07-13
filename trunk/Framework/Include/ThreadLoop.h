#ifndef __THREADLOOP_H__
#define __THREADLOOP_H__

#include "NoCopyable.h"
#include "SimpleIFaces.h"
#include "Pointers.h"
#include "Mutex.h"
#include "Thread.h"
#include "ManualEvent.h"

namespace System
{
  class ThreadLoop
    : public Common::NoCopyable
  {
  public:
    ThreadLoop(Thread::ThreadCallbackPtr callback);
    ~ThreadLoop();
    void Resume();
  private:
    Thread::ThreadCallbackPtr InternalCallback;
    volatile bool IsRun;
    Thread::ThreadCallbackPtr Callback;
    Mutex Mutex_;
    ManualEvent Event_;
    Common::SharedPtr<Thread> Thread_;
    void CallbackFunc();
  };
}
#endif  // !__THREADLOOP_H__
