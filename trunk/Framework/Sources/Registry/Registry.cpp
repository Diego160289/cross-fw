#include "Registry.h"
#include "SyncObj.h"
#include "File.h"
#include "Typedefs.h"
#include "IVariantImpl.h"
#include "INamedVariableImpl.h"
#include "IEnumImpl.h"
#include "CommonUtils.h"


#include <iterator>
#include <sstream>
#include <vector>


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

namespace
{
  std::string GetRegistryVersion(const TiXmlDocument *doc)
  {
    if (!doc)
      return std::string();
    const TiXmlElement *Registry = doc->FirstChildElement("Registry");
    if (!Registry)
      return std::string();
    std::string Ret;
    if (Registry->QueryValueAttribute("Version", &Ret) != TIXML_SUCCESS)
      return std::string();
    return Ret;
  }

  std::string GetKeysHash(const TiXmlDocument *doc)
  {
    if (!doc)
      return std::string();
    const TiXmlElement *Registry = doc->FirstChildElement("Registry");
    if (!Registry)
      return std::string();
    const TiXmlNode *Hash = Registry->FirstChildElement("Hash");
    if (!Hash)
      return std::string();
    const TiXmlNode *HashText = Hash->FirstChild();
    return !HashText ? std::string() : HashText->ValueStr();
  }

  TiXmlNode* OpenRegistry(TiXmlDocument *doc)
  {
    if (!doc)
      return 0;
    return doc->FirstChildElement("Registry");
  }

  TiXmlNode* OpenKeys(TiXmlDocument *doc)
  {
    if (!doc)
      return 0;
    TiXmlNode *Registry = OpenRegistry(doc);
    if (!Registry)
      return 0;
    return Registry->FirstChildElement("Keys");
  }

  TiXmlNode* OpenKey(TiXmlDocument *doc, const char *pathKey)
  {
    TiXmlNode *Keys = OpenKeys(doc);
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

  class RegistryHash
  {
  public:
    std::string Create(TiXmlDocument *doc)
    {
      const TiXmlNode *Keys = OpenKeys(doc);
      if (!Keys)
        return std::string();
      std::string Str;
      if (!CreateString(Keys->FirstChild(), &Str) || Str.length() < 1)
        return std::string();
      return Common::GetChecksumMD5(Str.c_str(), static_cast<unsigned>(Str.length()));
    }
  private:
    bool CreateString(const TiXmlNode *keys, std::string *str)
    {
      if (!keys)
        return false;
      for (const TiXmlNode *i = keys ; i ; i = i->NextSibling())
      {
        str->append(i->ValueStr());
        const TiXmlNode *Child = i->FirstChild();
        if (Child && !CreateString(Child, str))
          return false;
      }
      return true;
    }
  };
}

RetCode IRegistryImpl::InternalLoad(const char *registryPath)
{
  if (Document.Get())
    return retFail;
  TiXmlDocumentPtr NewDocumet(new TiXmlDocument);
  if (!NewDocumet->LoadFile(registryPath, TIXML_ENCODING_UTF8))
    return retFail;
  if (GetRegistryVersion(NewDocumet.Get()) != RegistryVersion ||
    GetKeysHash(NewDocumet.Get()) != RegistryHash().Create(NewDocumet.Get()))
  {
    return retFail;
  }
  Document.Swap(NewDocumet);
  IsModifiedState = false;
  return retOk;
}

RetCode IRegistryImpl::InternalUnload()
{
  if (!Document.Get())
    return retFail;
  Save();
  Document.Release();
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

const char* IRegistryImpl::GetCtrlVersion() const
{
  return RegistryVersion;
}

RetCode IRegistryImpl::CreateKey(const char *pathKey)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  TiXmlNode *Keys = OpenKeys(Document.Get());
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
      {
        IsModifiedState = true;
        SaveLoop->Resume();
      }
    }
    else
      Keys = CurKey;
  }
  return retOk;
}

RetCode IRegistryImpl::RemoveKey(const char *pathKey)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  TiXmlNode *Keys = OpenKeys(Document.Get());
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
  SaveLoop->Resume();
  return retOk;
}

RetCode IRegistryImpl::GetValue(const char *pathKey, IFaces::IVariant **value)
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  const TiXmlNode *Key = OpenKey(Document.Get(), pathKey);
  if (!Key)
    return retFail;
  try
  {
    Common::RefObjPtr<IFaces::IVariant> Var = IFacesImpl::IVariantImpl<System::Mutex>::CreateObject();
    const TiXmlNode *KeyValue = Key->FirstChild();
    if (KeyValue)
    {
      if (KeyValue->Type() != TiXmlNode::TEXT)
        return retFail;
      std::string Value = KeyValue->ValueStr();
      if (!Value.empty() && Var->FromBase64Pack(Value.c_str()) != retOk)
        return retFail;
    }
    if (Var.QueryInterface(value) != retOk)
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
  SaveLoop->Resume();
  return retOk;
}

namespace
{
  std::string ExtractKeyName(const TiXmlNode *node)
  {
    if (node->Type() == TiXmlNode::TEXT)
      return "";
    const TiXmlElement *Element = node->ToElement();
    if (!Element)
      return "";
    std::string KeyName = Element->ValueStr();
    if (KeyName.empty())
      return "";
    std::stringstream Io;
    Io << KeyName;
    Common::StringVector KeyPair;
    for (std::string s ; std::getline(Io, s, '_') ; KeyPair.push_back(s));
    if (KeyPair.size() < 2 || KeyPair.front() != "Key")
      return "";
    return KeyPair[1];
  }

  typedef IFacesImpl::IEnumImpl<System::Mutex>::ThisTypePtr EnumImplPtr;

  bool EnumKeys(const TiXmlNode *node, EnumImplPtr keys)
  {
    if (node->Type() == TiXmlNode::TEXT)
    {
      std::string ValueStr = node->ValueStr();
      if (ValueStr.empty())
        return false;
      Common::RefObjPtr<IFaces::IVariant> Value = IFacesImpl::CreateVariant<System::Mutex>();
      if (Value->FromBase64Pack(ValueStr.c_str()) != retOk)
        return false;
      const TiXmlNode *Parent = node->Parent();
      std::string ParentName = ExtractKeyName(Parent);
      if (ParentName.empty())
        return false;
      Common::RefObjPtr<IFaces::INamedVariable> Var = IFacesImpl::CreateNamedVariable<System::Mutex>(ParentName.c_str());
      if (Var->Set(Value.Get()) != retOk)
        return false;
      keys->AddItem(Var);
      return true;
    }
    for (const TiXmlNode *i = node->FirstChild() ; i ; i = i->NextSibling())
    {
      if (i->Type() == TiXmlNode::TEXT)
      {
        return EnumKeys(i, keys);
      }
      std::string KeyName = ExtractKeyName(i);
      if (KeyName.empty())
        return false;
      Common::RefObjPtr<IFaces::INamedVariable> Var = IFacesImpl::CreateNamedVariable<System::Mutex>(KeyName.c_str());
      Common::RefObjPtr<IFaces::IVariant> Value = IFacesImpl::CreateVariant<System::Mutex>();
      EnumImplPtr Keys = IFacesImpl::CreateEnum<System::Mutex>();
      {
        Common::RefObjPtr<IFaces::IBase> Item;
        if (Keys.QueryInterface(Item.GetPPtr()) != retOk ||
          Value->SetValue(IFaces::IVariant::vtIBase, Item.Get()) != retOk)
        {
          return false;
        }
      }
      if (Var->Set(Value.Get()) != retOk)
        return false;
      {
        Common::RefObjPtr<IFaces::IBase> Item;
        if (Var.QueryInterface(Item.GetPPtr()) != retOk)
          return false;
        keys->AddItem(Item);
      }
      const TiXmlNode *Child = i->FirstChild();
      if (Child && !EnumKeys(i, Keys))
        return false;
    }
    return true;
  }
}

RetCode IRegistryImpl::EnumKey(const char *pathKey, IFaces::IEnum **keys)
try
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  const TiXmlNode *Key = OpenKey(Document.Get(), pathKey);
  if (!Key)
    return retFail;
  if (!Key->FirstChild())
    return retFail;
  EnumImplPtr Keys = IFacesImpl::CreateEnum<System::Mutex>();
  return !EnumKeys(Key, Keys) || Keys.QueryInterface(keys) != retOk ? retFail : retOk;
}
catch (std::exception &)
{
  return retFail;
}

bool IRegistryImpl::FinalizeCreate()
{
  SaveLoop.Reset(new System::ThreadLoop(
    CreateMemberCakkback(*this, &IRegistryImpl::SaveRegistry)
    ));
  return true;
}

void IRegistryImpl::BeforeDestroy()
{
  SaveLoop.Release();
  
}

void IRegistryImpl::SaveRegistry()
{
  Common::SyncObject<System::Mutex> Locker(GetSynObj());
  Save();
}

void IRegistryImpl::Save()
{
  if (!Document.Get() || !IsModifiedState)
    return;
  std::string Hash = RegistryHash().Create(Document.Get());
  if (Hash.empty())
    return;
  TiXmlNode *Registry = OpenRegistry(Document.Get());
  if (!Registry)
    return;
  {
    TiXmlNode *HashNode = Registry->FirstChildElement("Hash");
    if (HashNode && !Registry->RemoveChild(HashNode))
      return;
  }
  TiXmlText HashText(Hash);
  HashText.SetCDATA(true);
  TiXmlElement HashNode("Hash");
  HashNode.InsertEndChild(HashText);
  Registry->InsertEndChild(HashNode);
  if (!Document->SaveFile())
    return;
  IsModifiedState = false;
}
