#ifndef __SERVICEMANAGER_H__
#define __SERVICEMANAGER_H__

#include "IFacesTools.h"
#include "Mutex.h"
#include "Pointers.h"
#include "ManualEvent.h"

#include <string>
#include <vector>
#include <map>

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

  IServiceManagerImpl();

  // IServiceManager
  virtual RetCode StartService(const char *serviceId, IFaces::IBase **service);
  virtual RetCode StartService(const char *serviceId);
  virtual RetCode StopService(const char *serviceId);
  virtual RetCode PostStopToService(const char *serviceId);
  virtual RetCode PostStopToServiceManager();
  virtual RetCode GetServicePool(const char *serviceId, IFaces::IEnum **services);

  // IServiceManagerCtrl
  virtual RetCode SetRegistry(IFaces::IRegistry *registry);
  virtual RetCode SetClassFactory(IFaces::IClassFactory *factory);
  virtual RetCode Run(const char *startServiceId);

  virtual void BeforeDestroy();
  virtual bool FinalizeCreate();

private:
  System::Mutex RegistryMtx;
  Common::RefObjPtr<IFaces::IRegistry> Registry;
  System::Mutex FactoryMtx;
  Common::RefObjPtr<IFaces::IClassFactory> Factory;

  typedef Common::RefObjPtr<IFaces::IService> IServicePtr;
  typedef std::vector<IServicePtr> ServicePool;
  typedef Common::SharedPtr<ServicePool> ServicePoolPtr;
  typedef std::map<std::string, ServicePoolPtr> ServiceMap;

  System::Mutex IsRunMtx;
  volatile bool IsRun;
  System::ManualEvent RunEvent;

  System::Mutex ServicesMtx;
  ServiceMap Services;

  IServicePtr InternalStartService(const std::string &serviceId);
  bool BuildService(IServicePtr service, const std::string &instanceUUID);
  void StopAllServices();
};

#endif  // !__SERVICEMANAGER_H__
