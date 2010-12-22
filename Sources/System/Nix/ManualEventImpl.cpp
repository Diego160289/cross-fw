//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "ManualEventImpl.h"


namespace System
{

  ManualEvent::ManualEventImpl::ManualEventImpl()
    : Flag(true)
  {
    if( pthread_mutex_init(&Mtx, 0))
      throw ManualEventException("Can't create manual event");
    if (pthread_cond_init(&CondVar, 0))
    {
      pthread_mutex_destroy(&Mtx);
      throw ManualEventException("Can't create manual event");
    }

  }

  ManualEvent::ManualEventImpl::~ManualEventImpl()
  {
    pthread_mutex_destroy(&Mtx);
    pthread_cond_destroy(&CondVar);
  }

  void ManualEvent::ManualEventImpl::Set()
  {
    if (pthread_mutex_lock(&Mtx))
      throw ManualEventException("Can't set event");
    bool OldFalg = Flag;
    Flag = true;
    if (pthread_cond_broadcast(&CondVar))
    {
      Flag = OldFalg;
      pthread_mutex_unlock(&Mtx);
      throw ManualEventException("Can't set event");
    }
    pthread_mutex_unlock(&Mtx);
  }

  void ManualEvent::ManualEventImpl::Reset()
  {
    if (pthread_mutex_lock(&Mtx))
      throw ManualEventException("Can't reset event");
    Flag = false;
    pthread_mutex_unlock(&Mtx);
  }

  bool ManualEvent::ManualEventImpl::Wait()
  {
    if (pthread_mutex_lock(&Mtx))
      throw ManualEventException("Error wait event");
    if(!Flag)
    {
      pthread_mutex_unlock(&Mtx);
      if(pthread_cond_wait(&CondVar, &Mtx))
        throw ManualEventException("Error wait event");
    }
    pthread_mutex_unlock(&Mtx);
    return true;
  }

}
