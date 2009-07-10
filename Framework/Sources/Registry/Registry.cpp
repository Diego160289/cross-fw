#include "Registry.h"
#include "SyncObj.h"
#include "File.h"
#include "Typedefs.h"
#include "IVariantImpl.h"
#include "INamedVariableImpl.h"
#include "IEnumImpl.h"


#include <iterator>
#include <sstream>
#include <vector>

#include <iostream>

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
  const TiXmlNode *Key = OpenKey(Document.Get(), pathKey);
  if (!Key)
    return retFail;
  try
  {
    Common::RefObjPtr<IFaces::IVariant> Var = IFacesImpl::IVariantImpl<Common::MultiObject, System::Mutex>::CreateObject();
    const TiXmlNode *KeyValue = Key->FirstChild();
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
  const TiXmlNode *Key = OpenKey(Document.Get(), pathKey);
  if (!Key)
    return retFail;
  const TiXmlNode *SubKeys = Key->FirstChild();
  if (!SubKeys)
    return retFail;
  if (SubKeys->Type() == TiXmlNode::TEXT)
    return retFail;
  try
  {
    std::vector<IFacesImpl::IEnumImpl<Common::MultiObject, System::Mutex>::ThisTypePtr> EnumStack;
    EnumStack.push_back(IFacesImpl::CreateEnum<System::Mutex>());
    Common::RefObjPtr<IFaces::IEnum> RetEnum(EnumStack.back());
    unsigned Level = 1;
    for (const TiXmlNode *i = SubKeys ; i ; )
    {
      const TiXmlNode *Next = 0;
      if (i->Type() != TiXmlNode::TEXT)
      {
        const TiXmlElement *Element = i->ToElement();
        if (!Element)
          return retFail;
        std::string KeyName = Element->ValueStr();
        if (KeyName.empty())
          return retFail;
        std::stringstream Io;
        Io << KeyName;
        Common::StringVector KeyPair;
        for (std::string s ; std::getline(Io, s, '_') ; KeyPair.push_back(s));
        if (KeyPair.size() != 2 || KeyPair.front() != "Key")
          return retFail;
        std::cout << KeyPair.back() << std::endl;
        const TiXmlNode *Child = i->FirstChild();
        if (Child)
        {
          i = Child;
          ++Level;
          Common::RefObjPtr<IFaces::INamedVariable> NewNamedVar = IFacesImpl::CreateNamedVariable<System::Mutex>(KeyPair.back().c_str());
          Common::RefObjPtr<IFaces::IVariant> NewVar = IFacesImpl::CreateVariant<System::Mutex>();
          {
            Common::RefObjPtr<IFaces::IEnum> CurEnum;
            if (!EnumStack.back().QueryInterface(&CurEnum))
              return retFail;
            IFacesImpl::IVariantHelper Helper(NewVar);
            Helper = static_cast<IFaces::IBase*>(CurEnum.Get());
          }
          if (NewNamedVar->Set(NewVar.Get()) != retOk)
            return retFail;
          EnumStack.push_back(IFacesImpl::CreateEnum<System::Mutex>());
          Common::RefObjPtr<IFaces::IBase> NewItem;
          if (!NewNamedVar.QueryInterface(&NewItem))
            return retFail;
          EnumStack.back()->AddItem(NewItem);
          continue;
        }
        else
        {
          Next = i->NextSibling();
          if (Next)
          {
            i = Next;
            continue;
          }
        }
      }
      if (i->Type() == TiXmlNode::TEXT)
      {
        std::string Value = i->ValueStr();
        if (Value.empty())
          return retFail;
        Common::RefObjPtr<IFaces::IVariant> Var = IFacesImpl::IVariantImpl<Common::MultiObject, System::Mutex>::CreateObject();
        if (Var->FromBase64Pack(Value.c_str()) != retOk)
          return retFail;
        //IFacesImpl::CreateNamedVariable<System::Mutex>(KeyPair.back().c_str(), 10);
      }
      const TiXmlNode *Parent = i->Parent();
      --Level;
      EnumStack.pop_back();
      Next = Parent->NextSibling();
      while (!Next && Level)
      {
        Parent = Parent->Parent();
        --Level;
        EnumStack.pop_back();
        Next = Parent->NextSibling();
      }
      i = Next;
    }
    if (!RetEnum.QueryInterface(enumKey))
      return retFail;
  }
  catch (std::exception &)
  {
    return retFail;
  }
  return retOk;
}
