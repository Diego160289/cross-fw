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
    ThreadLoop(Common::ICallbackPtr callback);
    ~ThreadLoop();
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
}
#endif  // !__THREADLOOP_H__
