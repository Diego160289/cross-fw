#include "ServiceManager.h"
#include "IEnumImpl.h"
#include "RefObjQIPtr.h"


IServiceManagerImpl::IServiceManagerImpl()
  : IsRun(false)
{
}

RetCode IServiceManagerImpl::StartService(const char *serviceId, IFaces::IBase **service)
{
  return retFail;
}

RetCode IServiceManagerImpl::StartService(const char *serviceId)
{
  return retFail;
}

RetCode IServiceManagerImpl::StopService(const char *serviceId)
{
  return retFail;
}

RetCode IServiceManagerImpl::PostStopToService(const char *serviceId)
{
  return retFail;
}

RetCode IServiceManagerImpl::PostStopToServiceManager()
{
  return retFail;
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
      Common::RefObjQIPtr<IFaces::IBase> CurService(*i);
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
  if (!startServiceId)
    return retFail;
  {
    Common::SyncObject<System::Mutex> Locker(IsRunMtx);
    if (IsRun)
      return retFail;
    {
      Common::SyncObject<System::Mutex> Locker(ServicesMtx);
      if (!InternalStartService(startServiceId).Get())
        return retFail;
    }
  }
  try
  {
    RunEvent.Reset();
    IsRun = true;
    while (!RunEvent.Wait());
    return retOk;
  }
  catch (std::exception &)
  {
    return retFail;
  }
  return retFail;
}

void IServiceManagerImpl::BeforeDestroy()
{
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
  return true;
}

IServiceManagerImpl::IServicePtr IServiceManagerImpl::InternalStartService(const std::string &serviceId)
{
  try
  {
    Common::RefObjPtr<IFaces::IBase> SrvObj;
    {
      Common::SyncObject<System::Mutex> Locker(FactoryMtx);
      if (Factory->CreateObject(serviceId.c_str(), SrvObj.GetPPtr()) != retOk)
        return IServicePtr();
    }
    Common::RefObjQIPtr<IFaces::IService> NewService(SrvObj);
    if (!NewService.Get())
      return IServicePtr();

    // TODO: build service

    if (NewService->Init() != retOk)
      return IServicePtr();
    {
      Common::SyncObject<System::Mutex> Locker(ServicesMtx);
      ServicePoolPtr &SrvPool = Services[serviceId];
      if (!SrvPool.Get())
        SrvPool.Reset(new ServicePool);
      SrvPool->push_back(NewService);
    }
    return NewService;
  }
  catch (std::exception &)
  {
    return IServicePtr();
  }
  return IServicePtr();
}
