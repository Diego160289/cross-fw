#include "IFacesTools.h"
#include "Mutex.h"

namespace IFacesTools
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
    return ModuleCounter::Instance.Counter->GetCounter();
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

  ModuleCounter ModuleCounter::Instance;
}
