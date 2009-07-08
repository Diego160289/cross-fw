#include "Registry.h"
#include "SyncObj.h"
#include "File.h"


const char IRegistryImpl::RegistryVersion[] = "1.0";

IRegistryImpl::IRegistryImpl()
  : IsModifiedState(false)
{
}

IRegistryImpl::~IRegistryImpl()
{
}

RetCode IRegistryImpl::Create(const char *registryPath)
{
  if (Document.Get())
    return retFail;
  TiXmlDocumentPtr NewDocument(new TiXmlDocument);
  TiXmlDeclaration Declaration("1.0", "utf-8", "yes");
  if (!NewDocument->InsertEndChild(Declaration))
    return retFail;
  TiXmlElement Registry("Registry");
  Registry.SetAttribute("Version", RegistryVersion);
  if (!Registry.InsertEndChild(TiXmlElement("Keys")))
    return retFail;
  if (!NewDocument->InsertEndChild(Registry))
    return retFail;
  if (!NewDocument->SaveFile(registryPath))
    return retFail;
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  if (Document.Get() && InternalUnload() != retOk ||
      InternalLoad(registryPath) != retOk)
  {
    return retFalse;
  }
  return retOk;
}

RetCode IRegistryImpl::Remove(const char *registryPath)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  Document.Release();
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

RetCode IRegistryImpl::InternalLoad(const char *registryPath)
{
  if (Document.Get())
    return retFail;
  TiXmlDocumentPtr NewDocumet(new TiXmlDocument);
  if (!NewDocumet->LoadFile(registryPath, TIXML_ENCODING_UTF8))
    return retFail;
  Document.Swap(NewDocumet);
  IsModifiedState = false;
  return retOk;
}

RetCode IRegistryImpl::InternalUnload()
{
  if (!Document.Get())
    return retFail;
  Document.Release();
  IsModifiedState = false;
  return retOk;
}

RetCode IRegistryImpl::Load(const char *registryPath)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  return InternalLoad(registryPath);
}

bool IRegistryImpl::IsLoaded() const
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  return !!Document.Get();
}

RetCode IRegistryImpl::Unload()
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  return InternalUnload();
}

RetCode IRegistryImpl::Save()
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  if (!Document.Get())
    return retFail;
  if (IsModifiedState)
  {
    if (!Document->SaveFile())
      return retFail;
  }
  IsModifiedState = false;
  return retOk;
}

bool IRegistryImpl::IsModified()
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  return IsModifiedState;
}

const char* IRegistryImpl::GetCtrlVersion() const
{
  return RegistryVersion;
}

const char* IRegistryImpl::GetLoadedRegistryVersion() const
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  if (!Document.Get())
    return 0;
  return 0;
}

RetCode IRegistryImpl::CreatePathKey(const char *pathKey)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
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
  return retOk;
}

RetCode IRegistryImpl::SetValue(const char *pathKey, IFaces::IVariant *value)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  return retOk;
}

RetCode IRegistryImpl::EnumKey(const char *pathKey, IFaces::IEnum **enumKey)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  return retOk;
}
