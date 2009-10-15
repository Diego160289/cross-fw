#ifndef __TESTSERVICE2_H__
#define __TESTSERVICE2_H__

#include "Timer.h"
#include "Pointers.h"

#include "ServiceBase.h"
#include "ILogObjectImpl.h"



class ITestService2Impl
  : public Common::ServiceBase
      <
        TYPE_LIST_2
          (
            IFaces::ITestService2,
            IFacesImpl::ILogObjectImpl
          )
      >
{
public:
  DECLARE_UUID(12457697-5839-4bb4-926a-8b5e37abcd6c)

  ITestService2Impl();

  virtual bool OnInit();
  virtual void OnDone();

  bool FinalizeCreate();
  void BeforeDestroy();

  // ITestService2
  virtual void Test();

private:

  Common::SharedPtr<System::Timer> Timer;
  unsigned Ticks;
  void OnTimer();
};

#endif  // !__TESTSERVICE2_H__
