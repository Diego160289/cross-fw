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
  std::cout << "IRegistryImpl::Create" << std::endl;
  return retOk;
}

RetCode IRegistryImpl::Load(const char *registryPath)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  std::cout << "IRegistryImpl::Load" << std::endl;
  return retOk;
}

RetCode IRegistryImpl::Save(const char *registryPath)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  std::cout << "IRegistryImpl::Save" << std::endl;
  return retOk;
}

RetCode IRegistryImpl::IsModified()
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  std::cout << "IRegistryImpl::IsModified" << std::endl;
  return retOk;
}

RetCode IRegistryImpl::Close()
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  std::cout << "IRegistryImpl::Close" << std::endl;
  return retOk;
}

RetCode IRegistryImpl::CreatePathKey(const char *pathKey)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  std::cout << "IRegistryImpl::CreatePathKey" << std::endl;
  return retOk;
}

RetCode IRegistryImpl::GetValue(const char *pathKey, IFaces::IVariant **value)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  std::cout << "IRegistryImpl::GetValue" << std::endl;
  return retOk;
}

RetCode IRegistryImpl::SetValue(const char *pathKey, IFaces::IVariant *value)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  std::cout << "IRegistryImpl::SetValue" << std::endl;
  return retOk;
}

RetCode IRegistryImpl::EnumKey(const char *pathKey, IFaces::IEnum **enumKey)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  std::cout << "IRegistryImpl::EnumKey" << std::endl;
  return retOk;
}
