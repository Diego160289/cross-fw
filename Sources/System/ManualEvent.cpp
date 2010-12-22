//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "ManualEvent.h"
#include "ManualEventImpl.h"


namespace System
{
  ManualEvent::ManualEvent()
    : Impl(new ManualEventImpl)
  {
  }

  ManualEvent::~ManualEvent()
  {
    delete Impl;
  }

  void ManualEvent::Set()
  {
    Impl->Set();
  }

  void ManualEvent::Reset()
  {
    Impl->Reset();
  }

  bool ManualEvent::Wait()
  {
    return Impl->Wait();
  }
}
