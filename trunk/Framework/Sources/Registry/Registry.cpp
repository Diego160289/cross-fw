#include "Registry.h"
#include "SyncObj.h"

IRegistryImpl::IRegistryImpl()
{
}

IRegistryImpl::~IRegistryImpl()
{
}

RetCode IRegistryImpl::Open(const char *registryPath)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  return retOk;
}

RetCode IRegistryImpl::Close()
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  return retOk;
}

RetCode IRegistryImpl::IsOpen()
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  return retOk;
}
