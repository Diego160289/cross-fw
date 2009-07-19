#ifndef __TESTSERVICE1_H__
#define __TESTSERVICE1_H__

#include "Timer.h"
#include "Pointers.h"

#include "ServiceBase.h"

class ITestService1Impl
  : public Common::ServiceBase
      <
        ITestService1Impl,
        TYPE_LIST_1(IFaces::ITestService1)
      >
{
public:
  DECLARE_UUID(725d19bb-e6bc-4a42-986d-721f0fb1c5d6)

  ITestService1Impl();

  virtual bool OnInit();

  // ITestService1
  virtual void Test();

private:

  Common::SharedPtr<System::Timer> Timer;
  unsigned Ticks;
  void OnTimer();
};

#endif  // !__TESTSERVICE1_H__
