#include "ServiceManager.h"
#include "IEnumImpl.h"
#include "RefObjQIPtr.h"
#include "IVarMapImpl.h"
#include "ServiceParamNames.h"

#include <algorithm>

const unsigned IServiceManagerImpl::ServiceCleanerTimeout = 5000;

IServiceManagerImpl::IServiceManagerImpl()
  : InstanceId(0)
{
}

IServiceManagerImpl::~IServiceManagerImpl()
{
}

unsigned long IServiceManagerImpl::StartService(const char *serviceId,
                                                IFaces::IBase **newServiceInstance)
{
  if (!serviceId)
    return retBadParam;
  unsigned long NewInstanceId = 0;
  IServicePtr NewService = InternalStartService(serviceId, 0, &NewInstanceId);
  if (!NewService.Get())
    return 0;
  if (NewService.QueryInterface(newServiceInstance) != retOk)
  {
    DoneService(ServicePair(NewService, std::vector<unsigned long>()));
    return 0;
  }
  return NewInstanceId;
}

unsigned long IServiceManagerImpl::StartService(const char *serviceId)
{
  if (!serviceId)
    return retBadParam;
  unsigned long NewInstanceId = 0;
  IServicePtr NewService = InternalStartService(serviceId, 0, &NewInstanceId);
  return !NewService.Get() ? 0 : NewInstanceId;
}

RetCode IServiceManagerImpl::StopService(unsigned long instanceId)
{
  if (!instanceId)
    return retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  for (ServiceMap::iterator i = Services.begin() ; i != Services.end() ; ++i)
  {
    ServicePool::iterator InstanceIter = i->second->find(instanceId);
    if (InstanceIter == i->second->end())
      continue;
    DoneService(InstanceIter->second);
    i->second->erase(InstanceIter);
    return retOk;
  }
  return retFail;
}

RetCode IServiceManagerImpl::PostStopToService(unsigned long instanceId)
{
  if (!instanceId)
    return retBadParam;
  Common::ISyncObject Locker(GetSynObj());
  for (ServiceMap::iterator i = Services.begin() ; i != Services.end() ; ++i)
  {
    ServicePool::iterator InstanceIter = i->second->find(instanceId);
    if (InstanceIter == i->second->end())
      continue;
    StoppingServices.push_back(InstanceIter->second);
    i->second->erase(InstanceIter);
    break;
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
  Common::ISyncObject Locker(GetSynObj());
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
  Common::ISyncObject Locker(GetSynObj());
  if (!serviceId)
    return retBadParam;
  ServiceMap::iterator ServiceGroup = Services.find(serviceId);
  if (ServiceGroup == Services.end())
    return retFail;
  for (ServicePool::const_iterator i = ServiceGroup->second->begin() ; i != ServiceGroup->second->end() ; ++i)
    StoppingServices.push_back(i->second);
  Services.erase(ServiceGroup);
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
    Common::ISyncObject Locker(GetSynObj());
    ServiceMap::iterator Iter = Services.find(serviceId);
    if (Iter == Services.end())
      return retFalse;
    Common::RefObjPtr<IFacesImpl::IEnumImpl> ServiceEnum =
      IFacesImpl::CreateEnum<System::Mutex>();
    ServicePoolPtr SrvPool = Iter->second;
    for (ServicePool::iterator i = SrvPool->begin() ; i != SrvPool->end() ; ++i)
    {
      Common::RefObjQIPtr<IFaces::IBase> CurService(i->second.first);
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
  Common::ISyncObject Locker(GetSynObj());
  Registry = registry;
  return retOk;
}

RetCode IServiceManagerImpl::SetClassFactory(IFaces::IClassFactory *factory)
{
  Common::ISyncObject Locker(GetSynObj());
  Factory = factory;
  return retOk;
}

RetCode IServiceManagerImpl::Run(const char *startServiceId)
{
  bool StartedFirstService = false;
  RetCode Ret = retOk;
  if (!startServiceId)
    return retFail;
  unsigned long NewInstanceId = 0;
  if (!(StartedFirstService = !!InternalStartService(startServiceId, 0, &NewInstanceId).Get()))
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

      StopManager();
      return retOk;
    }
    if (StartedFirstService)
      StopManager();
    return retFail;
  }
  catch (std::exception &)
  {
    if (StartedFirstService)
      StopManager();
    return retFail;
  }
  return retFail;
}

void IServiceManagerImpl::BeforeDestroy()
{
  StopManager();
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

IServiceManagerImpl::IServicePtr IServiceManagerImpl::InternalStartService(const std::string &serviceId,
                                                                           IFaces::IBase *parent,
                                                                           unsigned long *instanceId)
{
  Common::ISyncObject Locker(GetSynObj());
  try
  {
    Common::RefObjPtr<IFaces::IBase> SrvObj;
    if (Factory->CreateObject(serviceId.c_str(), SrvObj.GetPPtr()) != retOk)
      return IServicePtr();
    Common::RefObjQIPtr<IFaces::IService> NewService(SrvObj);
    ServicePair NewServicePair = std::make_pair(NewService, std::vector<unsigned long>());
    if (!NewService.Get() ||!BuildService(NewServicePair))
      return IServicePtr();
    if (NewService->Init() != retOk)
    {
      UnbuildService(NewServicePair);
      return IServicePtr();
    }
    ServicePoolPtr &SrvPool = Services[serviceId];
    if (!SrvPool.Get())
      SrvPool.Reset(new ServicePool);
    (*SrvPool.Get())[(*instanceId = ++InstanceId)] = NewServicePair;
    return NewService;
  }
  catch (std::exception &)
  {
    return IServicePtr();
  }
  return IServicePtr();
}

bool IServiceManagerImpl::BuildService(const ServicePair &service)
{
  try
  {
    Common::RefObjQIPtr<IFaces::IEnvironment> ServiceEnvironment(service.first);
    if (ServiceEnvironment.Get())
    {
      int k = 0;
    }
    Common::RefObjPtr<IFaces::IVarMap> VarMap = IFacesImpl::CreateVarMap<System::Mutex>();
    IFacesImpl::IVarMapHelper Params(VarMap);
    Params.AddVariable(IFacesImpl::PrmClassFactorry, Common::RefObjQIPtr<IFaces::IBase>(Factory).Get());
    Params.AddVariable(IFacesImpl::PrmServiceManager, GetThisIBase().Get());
    if (service.first->SetParams(VarMap.Get()) != retOk)
    {
      UnbuildService(service);
      return false;
    }

    // TODO: ���������� ��� ����������� ��������� �������
  }
  catch (std::exception &)
  {
    return false;
  }
  return true;
}

void IServiceManagerImpl::UnbuildService(const ServicePair &service)
{
  service.first->SetParams(0);
}

void IServiceManagerImpl::StopManager()
{
  CleanThread.Release();
  StopServiceThread.Release();
  Common::ISyncObject Locker(GetSynObj());
  Factory.Release();
  Registry.Release();

  for (ServiceMap::iterator i = Services.begin() ; i != Services.end() ; ++i)
  {
    for (ServicePool::iterator j = i->second->begin() ; j != i->second->end() ; ++j)
      DoneService(j->second);
  }
  Services.clear();
  StoppingServicesFunc();
}

void IServiceManagerImpl::DoneService(const ServicePair &service)
{
  // TODO: done childs
  service.first->Done();
  UnbuildService(service);
}

void IServiceManagerImpl::StoppingServicesFunc()
{
  Common::ISyncObject Locker(GetSynObj());
  for(ServicesVector::iterator i = StoppingServices.begin() ; i != StoppingServices.end() ; ++i)
    DoneService(*i);
  StoppingServices.clear();
}

void IServiceManagerImpl::ServiceCleanerFunc()
{
  try
  {
    Common::ISyncObject Locker(GetSynObj());
    Common::StringVector StoppingServiceKeys;
    for (ServiceMap::iterator i = Services.begin() ; i != Services.end() ; ++i)
    {
      ServicesVector GroupStoppingServices;
      std::vector<unsigned long> GroupStoppingServiceKeys;
      ServicePoolPtr ServiceGroup = i->second;
      for (ServicePool::iterator j = ServiceGroup->begin() ; j != ServiceGroup->end() ; ++j)
      {
        if (j->second.first->CanDone())
        {
          GroupStoppingServices.push_back(j->second);
          GroupStoppingServiceKeys.push_back(j->first);
        }
      }
      for (std::vector<unsigned long>::const_iterator j = GroupStoppingServiceKeys.begin() ; j != GroupStoppingServiceKeys.end() ; ++j)
        ServiceGroup->erase(*j);
      if (ServiceGroup->empty())
        StoppingServiceKeys.push_back(i->first);
      std::copy(GroupStoppingServices.begin(), GroupStoppingServices.end(),
        std::back_inserter(StoppingServices));
    }
    for (Common::StringVector::const_iterator i = StoppingServiceKeys.begin() ; i != StoppingServiceKeys.end() ; ++i)
      Services.erase(*i);
    if (Services.empty())
      RunEvent.Set();
    StopServiceThread->Resume();
  }
  catch (std::exception &)
  {
  }
}
