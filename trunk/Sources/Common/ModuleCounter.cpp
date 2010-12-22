//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "IFacesTools.h"
#include "Mutex.h"
#include "Pointers.h"


namespace Common
{
  class ModuleCounter::CounterImpl
    : private Common::NoCopyable
  {
  public:
    CounterImpl()
      : Counter(0)
    {
    }
    void Inc()
    {
      Common::SyncObject<System::Mutex> Locker(Mtx);
      ++Counter;
    }
    void Dec()
    {
      Common::SyncObject<System::Mutex> Locker(Mtx);
      --Counter;
    }
    unsigned long GetCounter()
    {
      Common::SyncObject<System::Mutex> Locker(Mtx);
      return Counter;
    }
  private:
    System::Mutex Mtx;
    unsigned long Counter;
  };

  unsigned long ModuleCounter::GetModuleCounter()
  {
    return ModuleCounter::GetInstance().Counter->GetCounter();
  }

  ModuleCounter::ModuleCounter() :
    Counter(new CounterImpl)
  {
  }

  ModuleCounter::~ModuleCounter()
  {
    delete Counter;
  }

  void ModuleCounter::Inc()
  {
    Counter->Inc();
  }

  void ModuleCounter::Dec()
  {
    Counter->Dec();
  }

  ModuleCounter& ModuleCounter::GetInstance()
  {
    static AutoPtr<ModuleCounter> Instance;
    if (!Instance.Get())
      Instance.Reset(new ModuleCounter);
    return *Instance.Get();
  }
}
