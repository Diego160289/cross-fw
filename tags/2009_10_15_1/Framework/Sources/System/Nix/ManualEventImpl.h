#ifndef __MANUALEVENTIMPL_H__
#define __MANUALEVENTIMPL_H__

#include "ManualEvent.h"

#include <pthread.h>

namespace System
{

  class ManualEvent::ManualEventImpl
    : private Common::NoCopyable
  {
  public:
    ManualEventImpl();
    ~ManualEventImpl();
    void Set();
    void Reset();
    bool Wait();
  private:
    pthread_mutex_t Mtx;
    pthread_cond_t CondVar;
    bool Flag;
  };

}

#endif  // !__MANUALEVENTIMPL_H__
