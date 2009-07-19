#include "ServiceManager.h"
#include "IEnumImpl.h"
#include "RefObjQIPtr.h"
#include "SystemUtils.h"
#include "IVarMapImpl.h"
#include "ServiceParamNames.h"

#include <algorithm>
#include <functional>


const unsigned IServiceManagerImpl::ServiceCleanerTimeout = 5000;

IServiceManagerImpl::IServiceManagerImpl()
{
}

RetCode IServiceManagerImpl::StartService(const char *serviceId, IFaces::IBase **service)
{
  if (!serviceId)
    return retBadParam;
  return InternalStartService(serviceId).QueryInterface(service);
}

RetCode IServiceManagerImpl::StartService(const char *serviceId)
{
  if (!serviceId)
    return retBadParam;
  return InternalStartService(serviceId).Get() ? retOk : retFail;
}

RetCode IServiceManagerImpl::StopService(const char *instanceUUID)
{
  if (!instanceUUID)
    return retBadParam;
  Common::SyncObject<System::Mutex> Locker(ServicesMtx);
  for (ServiceMap::iterator i = Services.begin() ; i != Services.end() ; ++i)
  {
    ServicePool::iterator InstanceIter = i->second->find(instanceUUID);
    if (InstanceIter == i->second->end())
      continue;
    DoneService(InstanceIter->second);
    i->second->erase(InstanceIter);
    return retOk;
  }
  return retFail;
}

RetCode IServiceManagerImpl::PostStopToService(const char *instanceUUID)
{
  ServicesVector StoppingServicesPool;
  if (!instanceUUID)
    return retBadParam;
  {
    Common::SyncObject<System::Mutex> Locker(ServicesMtx);
    for (ServiceMap::iterator i = Services.begin() ; i != Services.end() ; ++i)
    {
      ServicePool::iterator InstanceIter = i->second->find(instanceUUID);
      if (InstanceIter == i->second->end())
        continue;
      StoppingServicesPool.push_back(InstanceIter->second);
      i->second->erase(InstanceIter);
      break;
    }
  }
  {
    Common::SyncObject<System::Mutex> Locker(StoppingServicesMtx);
    std::copy(StoppingServicesPool.begin(), StoppingServicesPool.end(),
      std::back_inserter(StoppingServices));
  }
  try
  {
    StopServiceThread->Resume();
  }
  catch (std::exception &)
  {
  }
  return retOk;
}

RetCode IServiceManagerImpl::StopServiceGroup(const char *serviceId)
{
  if (!serviceId)
    return retBadParam;
  Common::SyncObject<System::Mutex> Locker(ServicesMtx);
  ServiceMap::iterator ServiceGroup = Services.find(serviceId);
  if (ServiceGroup == Services.end())
    return retFail;
  for (ServicePool::iterator i = ServiceGroup->second->begin() ; i != ServiceGroup->second->end() ; ++i)
    DoneService(i->second);
  Services.erase(ServiceGroup);
  return retOk;
}

RetCode IServiceManagerImpl::PostStopToServiceGroup(const char *serviceId)
{
  ServicesVector StoppingServicesPool;
  if (!serviceId)
    return retBadParam;
  {
    Common::SyncObject<System::Mutex> Locker(ServicesMtx);
    ServiceMap::iterator ServiceGroup = Services.find(serviceId);
    if (ServiceGroup == Services.end())
      return retFail;
    for (ServicePool::const_iterator i = ServiceGroup->second->begin() ; i != ServiceGroup->second->end() ; ++i)
      StoppingServicesPool.push_back(i->second);
    Services.erase(ServiceGroup);
    return retOk;
  }
  {
    Common::SyncObject<System::Mutex> Locker(StoppingServicesMtx);
    std::copy(StoppingServicesPool.begin(), StoppingServicesPool.end(),
      std::back_inserter(StoppingServices));
  }
  try
  {
    StopServiceThread->Resume();
  }
  catch (std::exception &)
  {
  }
  return retOk;
}

RetCode IServiceManagerImpl::PostStopToServiceManager()
{
  try
  {
    RunEvent.Set();
  }
  catch (std::exception &)
  {
    return retFail;
  }
  return retOk;
}

RetCode IServiceManagerImpl::GetServicePool(const char *serviceId, IFaces::IEnum **services)
{
  try
  {
    if (!serviceId)
      return retBadParam;
    Common::SyncObject<System::Mutex> Locker(ServicesMtx);
    ServiceMap::iterator Iter = Services.find(serviceId);
    if (Iter == Services.end())
      return retFalse;
    IFacesImpl::IEnumImpl<System::Mutex>::ThisTypePtr ServiceEnum = IFacesImpl::CreateEnum<System::Mutex>();
    ServicePoolPtr SrvPool = Iter->second;
    for (ServicePool::iterator i = SrvPool->begin() ; i != SrvPool->end() ; ++i)
    {
      Common::RefObjQIPtr<IFaces::IBase> CurService(i->second);
      if (!CurService.Get())
        return retFail;
      ServiceEnum->AddItem(CurService);
    }
    return ServiceEnum.QueryInterface(services);
  }
  catch (std::exception &)
  {
    return retFail;
  }
  return retFail;
}

RetCode IServiceManagerImpl::SetRegistry(IFaces::IRegistry *registry)
{
  Common::SyncObject<System::Mutex> Locker(RegistryMtx);
  Registry = registry;
  return retOk;
}

RetCode IServiceManagerImpl::SetClassFactory(IFaces::IClassFactory *factory)
{
  Common::SyncObject<System::Mutex> Locker(FactoryMtx);
  Factory = factory;
  return retOk;
}

RetCode IServiceManagerImpl::Run(const char *startServiceId)
{
  bool StartedFirstService = false;
  RetCode Ret = retOk;
  if (!startServiceId)
    return retFail;
  if (!(StartedFirstService = !!InternalStartService(startServiceId).Get()))
    return retFail;
  try
  {
    RunEvent.Reset();
  }
  catch (std::exception &)
  {
    Ret = retFail;
  }
  try
  {
    if (Ret == retOk)
    {
      while (!RunEvent.Wait());

      StopAllServices();
      return retOk;
    }
    if (StartedFirstService)
      StopAllServices();
    return retFail;
  }
  catch (std::exception &)
  {
    if (StartedFirstService)
      StopAllServices();
    return retFail;
  }
  return retFail;
}

void IServiceManagerImpl::BeforeDestroy()
{
  CleanThread.Release();
  StopServiceThread.Release();
  {
    Common::SyncObject<System::Mutex> Locker(FactoryMtx);
    Factory.Release();
  }
  {
    Common::SyncObject<System::Mutex> Locker(RegistryMtx);
    Registry.Release();
  }
}

bool IServiceManagerImpl::FinalizeCreate()
{
  StopServiceThread.Reset(new System::ThreadLoop(
    Common::CreateMemberCallback<IServiceManagerImpl>(*this, &IServiceManagerImpl::StoppingServicesFunc)
    ));
  CleanThread.Reset(new System::PulsedLoop(
    Common::CreateMemberCallback<IServiceManagerImpl>(*this, &IServiceManagerImpl::ServiceCleanerFunc),
    ServiceCleanerTimeout
    ));
  return true;
}

IServiceManagerImpl::IServicePtr IServiceManagerImpl::InternalStartService(const std::string &serviceId)
{
  try
  {
    std::string InstanceUUID = System::GenerateUUID();
    Common::RefObjPtr<IFaces::IBase> SrvObj;
    {
      Common::SyncObject<System::Mutex> Locker(FactoryMtx);
      if (Factory->CreateObject(serviceId.c_str(), SrvObj.GetPPtr()) != retOk)
        return IServicePtr();
    }
    Common::RefObjQIPtr<IFaces::IService> NewService(SrvObj);
    if (!NewService.Get() ||!BuildService(NewService, InstanceUUID))
      return IServicePtr();;
    if (NewService->Init() != retOk)
    {
      UnbuildService(NewService);
      return IServicePtr();
    }
    {
      Common::SyncObject<System::Mutex> Locker(ServicesMtx);
      ServicePoolPtr &SrvPool = Services[serviceId];
      if (!SrvPool.Get())
        SrvPool.Reset(new ServicePool);
      (*SrvPool.Get())[InstanceUUID] = NewService;
    }
    return NewService;
  }
  catch (std::exception &)
  {
    return IServicePtr();
  }
  return IServicePtr();
}

bool IServiceManagerImpl::BuildService(IServicePtr service, const std::string &instanceUUID)
{
  try
  {
    Common::RefObjPtr<IFaces::IVarMap> VarMap = IFacesImpl::CreateVarMap<System::Mutex>();
    IFacesImpl::IVarMapHelper Params(VarMap);
    Params.AddVariable(IFacesImpl::PrmClassFactorry, Common::RefObjQIPtr<IFaces::IBase>(Factory).Get());
    Params.AddVariable(IFacesImpl::PrmServiceManager, Common::RefObjQIPtr<IFaces::IBase>(this).Get());
    if (service->SetParams(VarMap.Get()) != retOk)
    {
      UnbuildService(service);
      return false;
    }
    service->SetInstanceUUID(instanceUUID.c_str());

    // TODO: установить все зависимости окружения сервиса
  }
  catch (std::exception &)
  {
    return false;
  }
  return true;
}

void IServiceManagerImpl::UnbuildService(IServicePtr service)
{
  service->SetParams(0);
  service->SetInstanceUUID(0);
}

void IServiceManagerImpl::StopAllServices()
{
  Common::SyncObject<System::Mutex> Locker(ServicesMtx);
  for (ServiceMap::iterator i = Services.begin() ; i != Services.end() ; ++i)
  {
    for (ServicePool::iterator j = i->second->begin() ; j != i->second->end() ; ++j)
      DoneService(j->second);
  }
  Services.clear();
}

void IServiceManagerImpl::DoneService(IServicePtr service)
{
  UnbuildService(service);
  service->Done();
}

void IServiceManagerImpl::StoppingServicesFunc()
{
  Common::SyncObject<System::Mutex> Locker(StoppingServicesMtx);
  std::for_each(StoppingServices.begin(), StoppingServices.end(),
    std::bind1st(std::mem_fun(&IServiceManagerImpl::DoneService), this));
  StoppingServices.clear();
}

void IServiceManagerImpl::ServiceCleanerFunc()
{
  try
  {
    ServicesVector AllStoppingServices;
    {
      Common::SyncObject<System::Mutex> Locler(ServicesMtx);
      Common::StringVector StoppingServiceKeys;
      for (ServiceMap::iterator i = Services.begin() ; i != Services.end() ; ++i)
      {
        ServicesVector GroupStoppingServices;
        Common::StringVector GroupStoppingServiceKeys;
        ServicePoolPtr ServiceGroup = i->second;
        for (ServicePool::iterator j = ServiceGroup->begin() ; j != ServiceGroup->end() ; ++j)
        {
          if (j->second->CanDone())
          {
            GroupStoppingServices.push_back(j->second);
            GroupStoppingServiceKeys.push_back(j->first);
          }
        }
        for (Common::StringVector::const_iterator j = GroupStoppingServiceKeys.begin() ; j != GroupStoppingServiceKeys.end() ; ++j)
          ServiceGroup->erase(*j);
        if (ServiceGroup->empty())
          StoppingServiceKeys.push_back(i->first);
        std::copy(GroupStoppingServices.begin(), GroupStoppingServices.end(),
          std::back_inserter(AllStoppingServices));
      }
      for (Common::StringVector::const_iterator i = StoppingServiceKeys.begin() ; i != StoppingServiceKeys.end() ; ++i)
        Services.erase(*i);
      if (Services.empty())
        RunEvent.Set();
    }
    Common::SyncObject<System::Mutex> Locker(StoppingServicesMtx);
    std::copy(AllStoppingServices.begin(), AllStoppingServices.end(),
      std::back_inserter(StoppingServices));
    StopServiceThread->Resume();
  }
  catch (std::exception &)
  {
  }
}
