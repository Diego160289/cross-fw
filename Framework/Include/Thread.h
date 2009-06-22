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
  private:
    typedef Common::SharedPtr<Common::ICallback> ThreadCallbackPtr;
    Thread(ThreadCallbackPtr callback);
    ~Thread();
  private:
    ThreadCallbackPtr Callback;
    class ThreadImpl;
    ThreadImpl *Impl;
  };
}

#endif  // !__THREAD_H__
