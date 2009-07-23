#ifndef __TESTSERVICE_H__
#define __TESTSERVICE_H__

#include "Timer.h"
#include "Pointers.h"

#include "ServiceBase.h"
#include "PropertiesImpl.h"
#include "ILogObjectImpl.h"


class ITestServiceImpl
  : public Common::ServiceBase
      <
        ITestServiceImpl,
        Common::MultiObject,
        TYPE_LIST_1(IFaces::ITestService)
      >
//  , public IFacesImpl::IPersistsPropertiesImpl<System::Mutex>
//  , public IFacesImpl::ILogObjectImpl<System::Mutex>
{ 
public:
  DECLARE_UUID(95e2d527-0cbf-41be-91f6-9ad3fca30f41)

  ITestServiceImpl();

  virtual bool OnInit();
  virtual void OnDone();

  // ITestService
  virtual void Test();

  bool FinalizeCreate();
  void BeforeDestroy();

private:

  Common::SharedPtr<System::Timer> Timer;
  unsigned Ticks;
  void OnTimer();
};

#endif  // !__TESTSERVICE_H__
