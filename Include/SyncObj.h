//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __SYNCOBJ_H__
#define __SYNCOBJ_H__

#include "NoCopyable.h"


namespace Common
{
  template <typename TSyn>
  class SyncObject
  : private Common::NoCopyable
  {
  public:
    SyncObject(TSyn &object)
      : Object(object)
    {
      Object.Lock();
    }
    ~SyncObject()
    {
      Object.Unlock();
    }
  private:
    TSyn &Object;
  };
}

#endif	// !__SYNCOBJ_H__
