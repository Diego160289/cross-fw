//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __SERVICEMANAGER_H__
#define __SERVICEMANAGER_H__

#include "IFacesTools.h"
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
        Common::TypeListAdapter
          <
            IFaces::IServiceManager,
            IFaces::IServiceManagerCtrl
          >
      >
{
public:
  DECLARE_UUID(74a12748-ee4a-4828-a502-6d2c05df637d)

  IServiceManagerImpl();
  virtual ~IServiceManagerImpl();

  // IServiceManager
  virtual unsigned long StartService(const char *serviceId, IFaces::IBase **newServiceInstance);
  virtual unsigned long StartService(const char *serviceId);
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

  Common::RefObjPtr<IFaces::IRegistry> Registry;
  Common::RefObjPtr<IFaces::IClassFactory> Factory;

  typedef Common::RefObjPtr<IFaces::IService> IServicePtr;
  typedef std::pair<IServicePtr/*instance*/, std::vector<unsigned long> /*Child Services*/> ServicePair;
  typedef std::vector<ServicePair> ServicesVector;
  ServicesVector StoppingServices;
  typedef std::map<unsigned long/*Instance Id*/, ServicePair> ServicePool;
  typedef Common::SharedPtr<ServicePool> ServicePoolPtr;
  typedef std::map<std::string/*Service UUID*/, ServicePoolPtr> ServiceMap;

  System::ManualEvent RunEvent;

  Common::SharedPtr<System::ThreadLoop> StopServiceThread;
  Common::SharedPtr<System::PulsedLoop> CleanThread;

  unsigned long InstanceId;
  ServiceMap Services;

  IServicePtr InternalStartService(const std::string &serviceId,
    IFaces::IBase *parent, unsigned long *instanceId);
  bool BuildService(const ServicePair &service);
  void UnbuildService(const ServicePair &service);
  void StopManager();
  void DoneService(const ServicePair &service);
  void StoppingServicesFunc();
  void ServiceCleanerFunc();
};

#endif  // !__SERVICEMANAGER_H__
