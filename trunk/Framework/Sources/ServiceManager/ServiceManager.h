#ifndef __SERVICEMANAGER_H__
#define __SERVICEMANAGER_H__

#include "IFacesTools.h"
#include "Mutex.h"
#include "Pointers.h"
#include "ManualEvent.h"
#include "ThreadLoop.h"
#include "PulsedLoop.h"

#include <string>
#include <map>
#include <vector>

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
  virtual unsigned long StartService(const char *serviceId, IFaces::IBase **service);
  virtual RetCode StartService(const char *serviceId);
  virtual RetCode StopService(unsigned long instanceId);
  virtual RetCode PostStopToService(unsigned long instanceId);
  virtual RetCode StopServiceGroup(const char *serviceId);
  virtual RetCode PostStopToServiceGroup(const char *serviceId);
  virtual RetCode PostStopToServiceManager();
  virtual RetCode GetServicePool(const char *serviceId, IFaces::IEnum **services);

  // IServiceManagerCtrl
  virtual RetCode SetRegistry(IFaces::IRegistry *registry);
  virtual RetCode SetClassFactory(IFaces::IClassFactory *factory);
  virtual RetCode Run(const char *startServiceId);

  virtual void BeforeDestroy();
  virtual bool FinalizeCreate();

private:
  static const unsigned ServiceCleanerTimeout;

  System::Mutex RegistryMtx;
  Common::RefObjPtr<IFaces::IRegistry> Registry;
  System::Mutex FactoryMtx;
  Common::RefObjPtr<IFaces::IClassFactory> Factory;

  typedef Common::RefObjPtr<IFaces::IService> IServicePtr;
  typedef std::map<std::string/*InstanceUUID*/, IServicePtr> ServicePool;
  typedef Common::SharedPtr<ServicePool> ServicePoolPtr;
  typedef std::map<std::string/*ServiceUUID*/, ServicePoolPtr> ServiceMap;

  System::ManualEvent RunEvent;

  Common::SharedPtr<System::PulsedLoop> CleanThread;

  System::Mutex ServicesMtx;
  ServiceMap Services;

  Common::SharedPtr<System::ThreadLoop> StopServiceThread;
  System::Mutex StoppingServicesMtx;
  typedef std::vector<IServicePtr> ServicesVector;
  ServicesVector StoppingServices;

  IServicePtr InternalStartService(const std::string &serviceId);
  bool BuildService(IServicePtr service, const std::string &instanceUUID);
  void UnbuildService(IServicePtr service);
  void StopAllServices();
  void DoneService(IServicePtr service);
  void StoppingServicesFunc();
  void ServiceCleanerFunc();
};

#endif  // !__SERVICEMANAGER_H__
