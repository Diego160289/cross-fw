#ifndef __TESTSERVICE_H__
#define __TESTSERVICE_H__

#include "IFacesTools.h"
#include "Mutex.h"

#include "Timer.h"
#include "Pointers.h"

using IFaces::RetCode;
using IFaces::retFail;
using IFaces::retOk;
using IFaces::retFalse;
using IFaces::retBadParam;
using IFaces::retNotImpl;

class ITestServiceImpl
  : public Common::CoClassBase
      <
        ITestServiceImpl,
        TYPE_LIST_1(IFaces::IService),
        Common::MultiObject,
        System::Mutex
      >
{
public:
  DECLARE_UUID(95e2d527-0cbf-41be-91f6-9ad3fca30f41)

  ITestServiceImpl();

  // IService
  virtual void SetInstanceUUID(const char *instanceUUID);
  virtual RetCode SetParams(IFaces::IVarMap *params);
  virtual RetCode Init();
  virtual void Done();
  virtual bool CanDone() const;

private:

  Common::SharedPtr<System::Timer> Timer;
  unsigned Ticks;
  void OnTimer();
  Common::RefObjPtr<IFaces::IServiceManager> Manager;
};

#endif  // !__TESTSERVICE_H__
