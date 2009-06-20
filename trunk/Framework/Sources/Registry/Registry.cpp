#include "Registry.h"
#include "SyncObj.h"

#include <iostream>

IRegistryImpl::IRegistryImpl()
{
}

IRegistryImpl::~IRegistryImpl()
{
}

RetCode IRegistryImpl::Create(const char *registryPath)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  return retOk;
}

RetCode IRegistryImpl::Load(const char *registryPath)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  std::cout << registryPath << std::endl;
  return retOk;
}

RetCode IRegistryImpl::Save(const char *registryPath)
{
  return retOk;
}

RetCode IRegistryImpl::IsModified()
{
  return retOk;
}

RetCode IRegistryImpl::Close()
{
  return retOk;
}

RetCode IRegistryImpl::CreatePathKey(const char *pathKey)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  std::cout << "blya" << std::endl;
  return retOk;
}

RetCode IRegistryImpl::GetValue(const char *pathKey, IFaces::IVariant **value)
{
  return retOk;
}

RetCode IRegistryImpl::SetValue(const char *pathKey, IFaces::IVariant *value)
{
  return retOk;
}

RetCode IRegistryImpl::EnumKey(const char *pathKey, IFaces::IEnum **enumKey)
{
  return retOk;
}
