#include "Registry.h"
#include "SyncObj.h"
#include "File.h"

#include <iostream>

const char IRegistryImpl::RegistryVersion[] = "1.0";

IRegistryImpl::IRegistryImpl()
  : IsLoadedState(false)
  , IsModifiedState(false)
{
}

IRegistryImpl::~IRegistryImpl()
{
}

RetCode IRegistryImpl::Create(const char *registryPath)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  if (IsLoadedState)
    return retFail;
  TiXmlDeclaration Declaration("1.0", "utf-8", "yes");
  if (!Document.InsertEndChild(Declaration))
    return retFail;
  TiXmlElement Registry("Registry");
  Registry.SetAttribute("Version", RegistryVersion);
  if (!Registry.InsertEndChild(TiXmlElement("Keys")))
    return retFail;
  if (!Document.InsertEndChild(Registry))
    return retFail;
  if (!Document.SaveFile(registryPath))
    return retFail;
  IsLoadedState = true;
  IsModifiedState = false;
  return retOk;
}

RetCode IRegistryImpl::Remove(const char *registryPath)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  if (IsLoadedState)
    Document.Clear();
  IsLoadedState = false;
  IsModifiedState = false;
  try
  {
    System::File::Remove(registryPath);
  }
  catch (std::exception &)
  {
    return retFail;
  }
  return retOk;
}

RetCode IRegistryImpl::Load(const char *registryPath)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  std::cout << "IRegistryImpl::Load" << std::endl;
  return retOk;
}

bool IRegistryImpl::IsLoaded() const
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  return IsLoadedState;
}

RetCode IRegistryImpl::Unload()
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
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

const char* IRegistryImpl::GetCtrlVersion() const
{
  return RegistryVersion;
}

const char* IRegistryImpl::GetLoadedRegistryVersion() const
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  return 0;
}

RetCode IRegistryImpl::CreatePathKey(const char *pathKey)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  std::cout << "IRegistryImpl::CreatePathKey" << std::endl;
  return retOk;
}

RetCode IRegistryImpl::RemovePathKey(const char *pathKey)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
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
