#include "ServiceManager.h"
#include "IEnumImpl.h"
#include "RefObjQIPtr.h"
#include "SystemUtils.h"
#include "IVarMapImpl.h"
#include "ServiceParamNames.h"


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
  Common::SyncObject<System::Mutex> Locker(IsRunMtx);
  if (!IsRun)
    return retFail;
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
    if (!InternalStartService(startServiceId).Get())
      return retFail;
    try
    {
      RunEvent.Reset();
      IsRun = true;
    }
    catch (std::exception &)
    {
      return retFail;
    }
  }
  try
  {
    while (!RunEvent.Wait());
    StopAllServices();
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
    std::string InstanceUUID = System::GenerateUUID();
    Common::RefObjPtr<IFaces::IBase> SrvObj;
    {
      Common::SyncObject<System::Mutex> Locker(FactoryMtx);
      if (Factory->CreateObject(serviceId.c_str(), SrvObj.GetPPtr()) != retOk)
        return IServicePtr();
    }
    Common::RefObjQIPtr<IFaces::IService> NewService(SrvObj);
    if (!NewService.Get() || !BuildService(NewService, InstanceUUID) ||
      NewService->Init() != retOk)
    {
      return IServicePtr();
    }
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

bool IServiceManagerImpl::BuildService(IServicePtr service, const std::string &instanceUUID)
{
  try
  {
    Common::RefObjPtr<IFaces::IVarMap> VarMap = IFacesImpl::CreateVarMap<System::Mutex>();
    IFacesImpl::IVarMapHelper Params(VarMap);
    Params.AddVariable(IFacesImpl::PrmClassFactorry, Common::RefObjQIPtr<IFaces::IBase>(Factory).Get());
    Params.AddVariable(IFacesImpl::PrmServiceManager, Common::RefObjQIPtr<IFaces::IBase>(this).Get());
    if (service->SetParams(VarMap.Get()) != retOk)
      return false;
    service->SetInstanceUUID(instanceUUID.c_str());

    // TODO: установить все зависимости окружения сервиса
  }
  catch (std::exception &)
  {
    return false;
  }
  return true;
}

void IServiceManagerImpl::StopAllServices()
{
}
