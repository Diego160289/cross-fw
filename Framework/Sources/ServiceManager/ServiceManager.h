#ifndef __SERVICEMANAGER_H__
#define __SERVICEMANAGER_H__

#include "IFacesTools.h"
#include "Mutex.h"


using IFaces::RetCode;
using IFaces::retFail;
using IFaces::retOk;
using IFaces::retFalse;
using IFaces::retBadParam;
using IFaces::retNotImpl;

class IServiceManagerImpl
  : public Common::CoClassBase
      <
        IServiceManagerImpl,
        TYPE_LIST_2(IFaces::IServiceManager, IFaces::IServiceManagerCtrl),
        Common::MultiObject,
        System::Mutex
      >
{
public:
  DECLARE_UUID(74a12748-ee4a-4828-a502-6d2c05df637d)

  // IServiceManager
  virtual RetCode StartService(const char *serviceId, IFaces::IBase **service);
  virtual RetCode StartService(const char *serviceId);
  virtual RetCode StopService(const char *serviceId);
  virtual RetCode PostStopToService(const char *serviceId);
  virtual RetCode PostStopToServiceManager();
  virtual RetCode GetService(const char *serviceId, IFaces::IBase **service);

  // IServiceManagerCtrl
  virtual RetCode SetRegistry(IFaces::IRegistry *registry);
  virtual RetCode SetClassFactory(IFaces::IClassFactory *factory);
  virtual RetCode Run();

private:
};

#endif  // !__SERVICEMANAGER_H__
