#include "ServiceManager.h"

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

RetCode IServiceManagerImpl::GetService(const char *serviceId, IFaces::IBase **service)
{
  return retFail;
}

RetCode IServiceManagerImpl::SetRegistry(IFaces::IRegistry *registry)
{
  return retFail;
}

RetCode IServiceManagerImpl::SetClassFactory(IFaces::IClassFactory *factory)
{
  return retFail;
}

RetCode IServiceManagerImpl::Run()
{
  return retFail;
}
