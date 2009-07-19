#ifndef __TESTSERVICE_H__
#define __TESTSERVICE_H__

#include "Timer.h"
#include "Pointers.h"

#include "ServiceBase.h"

class ITestServiceImpl
  : public Common::ServiceBase<ITestServiceImpl>
{
public:
  DECLARE_UUID(95e2d527-0cbf-41be-91f6-9ad3fca30f41)

  ITestServiceImpl();

  virtual bool OnInit();

private:

  Common::SharedPtr<System::Timer> Timer;
  unsigned Ticks;
  void OnTimer();
};

#endif  // !__TESTSERVICE_H__
