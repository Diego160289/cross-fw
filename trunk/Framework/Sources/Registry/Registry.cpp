#include "Registry.h"
#include "SyncObj.h"
#include "File.h"
#include "Typedefs.h"
#include "IVariantImpl.h"


#include <iterator>
#include <sstream>

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
  if ((Document.Get() && InternalUnload() != retOk) ||
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
  const TiXmlElement *Registry = Document->FirstChildElement("Registry");
  if (!Registry)
    return 0;
  if (Registry->QueryValueAttribute("Version", &LoadedRegistryVersion) != TIXML_SUCCESS)
    return 0;
  return LoadedRegistryVersion.c_str();
}

RetCode IRegistryImpl::CreateKey(const char *pathKey)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  if (!Document.Get())
    return retFail;
  TiXmlElement *Registry = Document->FirstChildElement("Registry");
  if (!Registry)
    return retFail;
  TiXmlNode *Keys = Registry->FirstChildElement("Keys");
  if (!Keys)
    return retFail;
  std::stringstream Io;
  Io << pathKey;
  Common::StringVector KeyPool;
  for (std::string s ; std::getline(Io, s, '/') ; KeyPool.push_back(s));
  if (KeyPool.empty())
    return retFail;
  TiXmlNode *CurKey = 0;
  for (Common::StringVector::const_iterator i = KeyPool.begin() ; i != KeyPool.end() ; ++i)
  {
    std::string CurKeyName = "Key_" + (*i);
    CurKey = Keys->FirstChildElement(CurKeyName.c_str());
    if (!CurKey)
    {
      const TiXmlNode *Item = Keys->FirstChild();
      if (Item && Item->Type() == TiXmlNode::TEXT)
        return retFail;
      if (!(Keys = Keys->InsertEndChild(TiXmlElement(CurKeyName.c_str()))))
        return retFail;
      else
        IsModifiedState = true;
    }
    else
      Keys = CurKey;
  }
  return retOk;
}

RetCode IRegistryImpl::RemoveKey(const char *pathKey)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  if (!Document.Get())
    return retFail;
  TiXmlElement *Registry = Document->FirstChildElement("Registry");
  if (!Registry)
    return retFail;
  TiXmlNode *Keys = Registry->FirstChildElement("Keys");
  if (!Keys)
    return retFail;
  std::stringstream Io;
  Io << pathKey;
  Common::StringVector KeyPool;
  for (std::string s ; std::getline(Io, s, '/') ; KeyPool.push_back(s));
  if (KeyPool.empty())
    return retFail;
  for (Common::StringVector::const_iterator i = KeyPool.begin() ; i != KeyPool.end() ; ++i)
  {
    std::string CurKeyName = "Key_" + (*i);
    if (!(Keys = Keys->FirstChildElement(CurKeyName.c_str())))
      return retFail;
  }
  if (!Keys)
    return retFail;
  TiXmlNode *Parent = Keys->Parent();
  if (!Parent || !Parent->RemoveChild(Keys))
    return retFail;
  IsModifiedState = true;
  return retOk;
}

namespace
{
  TiXmlNode* OpenKey(TiXmlDocument *doc, const char *pathKey)
  {
    if (!doc)
      return 0;
    TiXmlElement *Registry = doc->FirstChildElement("Registry");
    if (!Registry)
      return 0;
    TiXmlNode *Keys = Registry->FirstChildElement("Keys");
    if (!Keys)
      return 0;
    std::stringstream Io;
    Io << pathKey;
    Common::StringVector KeyPool;
    for (std::string s ; std::getline(Io, s, '/') ; KeyPool.push_back(s));
    if (KeyPool.empty())
      return 0;
    for (Common::StringVector::const_iterator i = KeyPool.begin() ; i != KeyPool.end() ; ++i)
    {
      std::string CurKeyName = "Key_" + (*i);
      if (!(Keys = Keys->FirstChildElement(CurKeyName.c_str())))
        return 0;
    }
    return Keys;
  }
}

RetCode IRegistryImpl::GetValue(const char *pathKey, IFaces::IVariant **value)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  TiXmlNode *Key = OpenKey(Document.Get(), pathKey);
  if (!Key)
    return retFail;
  try
  {
    Common::RefObjPtr<IFaces::IVariant> Var = IFacesImpl::IVariantImpl<Common::MultiObject, System::Mutex>::CreateObject();
    TiXmlNode *KeyValue = Key->FirstChild();
    if (KeyValue)
    {
      if (KeyValue->Type() != TiXmlNode::TEXT)
        return retFail;
      std::string Value = KeyValue->ValueStr();
      if (!Value.empty() && Var->FromBase64Pack(Value.c_str()) != retOk)
        return retFail;
    }
    if (!Var.QueryInterface(value))
      return retFail;
  }
  catch (std::exception &)
  {
    return retFail;
  }
  return retOk;
}

RetCode IRegistryImpl::SetValue(const char *pathKey, IFaces::IVariant *value)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  TiXmlNode *Key = OpenKey(Document.Get(), pathKey);
  if (!Key)
    return retFail;
  std::string Value = value->PackToBase64();
  if (Value.empty())
    return retFail;
  TiXmlText Text(Value);
  Text.SetCDATA(true);
  TiXmlNode *KeyValue = Key->FirstChild();
  if (KeyValue)
  {
    if (KeyValue->Type() != TiXmlNode::TEXT)
      return retFail;
    Key->RemoveChild(KeyValue);
  }
  Key->InsertEndChild(Text);
  IsModifiedState = true;
  return retOk;
}

RetCode IRegistryImpl::EnumKey(const char *pathKey, IFaces::IEnum **enumKey)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  return retOk;
}
