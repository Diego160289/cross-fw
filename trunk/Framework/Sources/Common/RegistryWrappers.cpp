#include "ComponentWrappers.h"
#include "RefObjQIPtr.h"

#include <sstream>


namespace Common
{
  namespace Wrappers
  {

    RegistryCtrl::RegistryCtrl(IRegistryCtrlPtr regCtrl)
      : RegCtrl(regCtrl)
    {
      if (!RegCtrl.Get())
        throw RegistryCtrException("Empty IRegistryCtrl pointer");
    }

    void RegistryCtrl::Create(const char *registryPath)
    {
      if (RegCtrl->Create(registryPath) != IFaces::retOk)
        throw RegistryCtrException("Can't create registry");
    }

    void RegistryCtrl::Remove(const char *registryPath)
    {
      if (RegCtrl->Remove(registryPath) != IFaces::retOk)
        throw RegistryCtrException("Can't remove registry");
    }

    void RegistryCtrl::Load(const char *registryPath)
    {
      if (RegCtrl->Load(registryPath) != IFaces::retOk)
        throw RegistryCtrException("Can't load registry");
    }

    bool RegistryCtrl::IsLoaded() const
    {
      return RegCtrl->IsLoaded();
    }

    void RegistryCtrl::Unload()
    {
      if (RegCtrl->Unload() != IFaces::retOk)
        throw RegistryCtrException("Can't unload registry");
    }


    Registry::Registry(IRegistryPtr reg)
      : Reg(reg)
    {
      if (!Reg.Get())
        throw RegistryException("Empty IRegistry pointer");
    }

    void Registry::CreateKey(const std::string &pathKey)
    {
      if (Reg->CreateKey(pathKey.c_str()) != IFaces::retOk)
        throw RegistryException("Can't create key");
    }

    void Registry::RemoveKey(const std::string &pathKey)
    {
      if (Reg->RemoveKey(pathKey.c_str()) != IFaces::retOk)
        throw RegistryException("Can't remove key");
    }

    const IFacesImpl::IVariantHelper Registry::GetValue(const std::string &pathKey) const
    {
      IFacesImpl::IVariantHelper::IVariantPtr Var;
      if (Reg->GetValue(pathKey.c_str(), Var.GetPPtr()) != IFaces::retOk)
        throw RegistryException("Can't get value");
      return Var;
    }

    void Registry::SetValue(const std::string &pathKey, const void *data, unsigned long bytes)
    {
      IFacesImpl::IVariantHelper::IVariantPtr Var = IFacesImpl::CreateVariant<System::Mutex>();
      {
        IFacesImpl::IVariantHelper Helper(Var);
        Helper.SetBinaryData(data, bytes);
      }
      if (Reg->SetValue(pathKey.c_str(), Var.Get()) != IFaces::retOk)
        throw RegistryException("Can't set value");
    }

    void Registry::SetValue(const std::string &pathKey, IFaces::IBase *iface)
    {
      IFacesImpl::IVariantHelper::IVariantPtr Var = IFacesImpl::CreateVariant<System::Mutex>();
      {
        IFacesImpl::IVariantHelper Helper(Var);
        Helper = iface;
      }
      if (Reg->SetValue(pathKey.c_str(), Var.Get()) != IFaces::retOk)
        throw RegistryException("Can't set value");
    }

    Registry::Key::KeyPtr Registry::EnumKey(const std::string &pathKey) const
    {
      IFacesImpl::IEnumHelper::IEnumPtr Enum;
      if (Reg->EnumKey(pathKey.c_str(), Enum.GetPPtr()) != IFaces::retOk)
        throw RegistryException("Can't enum key");
      return Key::KeyPtr(new Key(IFacesImpl::IEnumHelper(Enum), pathKey));
    }


    Registry::Key::Key(IFacesImpl::IEnumHelper keys, const std::string &keyName)
      : Keys(keys)
      , KeyName(keyName)
    {
    }

    bool Registry::Key::HasChildKeys() const
    {
      RefObjQIPtr<IFaces::INamedVariable> Var = Keys.First();
      if (!Var.Get())
        throw RegistryException("Can't get variable");
      RefObjPtr<IFaces::IVariant> Value;
      if (Var->Get(Value.GetPPtr()) != IFaces::retOk)
        throw RegistryException("Can't get value");
      if (Value->GetType() != IFaces::IVariant::vtIBase)
        return false;
      RefObjPtr<IFaces::IBase> ChildItem;
      if (Value->GetValue(reinterpret_cast<void**>(ChildItem.GetPPtr())) != IFaces::retOk ||
        !RefObjQIPtr<IFaces::IEnum>(ChildItem).Get())
      {
        throw RegistryException("Unknown child item");
      }
      return true;
    }

    bool Registry::Key::HasValue() const
    {
      RefObjQIPtr<IFaces::INamedVariable> Var = Keys.First();
      if (!Var.Get())
        throw RegistryException("Can't get variable");
      RefObjPtr<IFaces::IVariant> Value;
      if (Var->Get(Value.GetPPtr()) != IFaces::retOk)
        throw RegistryException("Can't get value");
      return Value->GetType() != IFaces::IVariant::vtIBase;
    }

    const std::string Registry::Key::GetKeyName() const
    {
      return KeyName;
    }

    const IFacesImpl::IVariantHelper Registry::Key::GetKeyValue() const
    {
      RefObjQIPtr<IFaces::INamedVariable> Var = Keys.First();
      if (!Var.Get())
        throw RegistryException("Can't get variable");
      RefObjPtr<IFaces::IVariant> Value;
      if (Var->Get(Value.GetPPtr()) != IFaces::retOk)
        throw RegistryException("Can't get value");
      if (Value->GetType() == IFaces::IVariant::vtIBase)
        throw RegistryException("Is not value");
      return Value;
    }

    const Registry::Key::KeysPoolPtr Registry::Key::GetChildKeys() const
    {
      KeysPoolPtr RetPool(new KeysPool);
      for (RefObjQIPtr<IFaces::INamedVariable> Var = Keys.First() ;
        Var.Get() ; Var = Keys.Next())
      {
        RefObjPtr<IFaces::IVariant> Value;
        if (Var->Get(Value.GetPPtr()) != IFaces::retOk)
          throw RegistryException("Can't get value");
        if (Value->GetType() != IFaces::IVariant::vtIBase)
          throw RegistryException("Unknown child item");
        RefObjPtr<IFaces::IBase> ChildItem;
        if (Value->GetValue(reinterpret_cast<void**>(ChildItem.GetPPtr())) != IFaces::retOk)
          throw RegistryException("Can't get child enum");
        RefObjQIPtr<IFaces::IEnum> Enum(ChildItem);
        if (!Enum.Get())
          throw RegistryException("Unknown child item");
        RetPool->push_back(KeyPtr(new Key(IFacesImpl::IEnumHelper(Enum), Var->GetName())));
      }
      return RetPool;
    }

    const char RegistryComponent::Key_ComponentInformation[] = "ComponentInformation";
    const char RegistryComponent::Key_ComponentClassIDs[] = "ComponentClassIDs";
    const char RegistryComponent::Key_Type[] = "Type";
    const char RegistryComponent::Key_ModuleName[] = "ModuleName";
    const char RegistryComponent::Key_Location[] = "Location";
    const char RegistryComponent::Key_Description[] = "Description";
    const char RegistryComponent::Key_ClassIDs[] = "ClassIDs";
    const char RegistryComponent::Key_ClassId[] = "ClassId";

    const char RegistryComponent::ComponentInfo::Type_Unknown[] = "Unknown";
    const char RegistryComponent::ComponentInfo::Type_InProc[] = "InProc";
    const char RegistryComponent::ComponentInfo::Type_OutProc[] = "OutProc";
    const char RegistryComponent::ComponentInfo::Type_Remote[] = "Remote";

          RegistryComponent::ComponentInfo::ComponentInfo()
      : Type(ctUnknown)
    {
    }

    void RegistryComponent::ComponentInfo::SetType(ComponentType type)
    {
      Type = type;
    }

    void RegistryComponent::ComponentInfo::SetType(const std::string &type)
    {
      if (type == Type_Unknown)
        Type = ctUnknown;
      else
      if (type == Type_InProc)
        Type = ctInProc;
      else
      if (type == Type_OutProc)
        Type = ctOutProc;
      else
      if (type == Type_Remote)
        Type = ctRemote;
      else
        throw RegistryException("Bad type");
    }

    RegistryComponent::ComponentInfo::ComponentType RegistryComponent::ComponentInfo::GetType() const
    {
      return Type;
    }

    const std::string RegistryComponent::ComponentInfo::GetTypeAsStr() const
    {
      switch (Type)
      {
      case ctInProc :
        return Type_InProc;
      case ctOutProc :
        return Type_OutProc;
      case ctRemote :
        return Type_Remote;
      default :
        break;
      }
      return Type_Unknown;
    }

    void RegistryComponent::ComponentInfo::SetModuleGuid(const std::string &guid)
    {
      ModuleGuid = guid;
    }

    const std::string& RegistryComponent::ComponentInfo::GetModuleGuid() const
    {
      return ModuleGuid;
    }

    void RegistryComponent::ComponentInfo::SetModuleName(const std::string &name)
    {
      ModuleName = name;
    }
    
    const std::string& RegistryComponent::ComponentInfo::GetModuleName() const
    {
      return ModuleName;
    }

    void RegistryComponent::ComponentInfo::SetLocation(const std::string &location)
    {
      Location = location;
    }

    const std::string& RegistryComponent::ComponentInfo::GetLocation() const
    {
      return Location;
    }

    void RegistryComponent::ComponentInfo::SetDescription(const std::string &description)
    {
      Description = description;
    }

    const std::string& RegistryComponent::ComponentInfo::GetDescription() const
    {
      return Description;
    }

    void RegistryComponent::ComponentInfo::AddClassId(const std::string &classId)
    {
      ClassIDs.push_back(classId);
    }

    const StringVector& RegistryComponent::ComponentInfo::GetClassIDs() const
    {
      return ClassIDs;
    }

    RegistryComponent::RegistryComponent(IRegistryPtr reg)
      : Reg(reg)
    {
    }

    RegistryComponent::ComponentInfoPtr RegistryComponent::GetComponentInfo(const std::string &classId) const
    {
      std::string Key = Key_ComponentClassIDs;
      Key += "/";
      std::string ModuleId = (const char*)Reg.GetValue(Key + classId);
      RegistryComponent::ComponentInfoPtr Info(new RegistryComponent::ComponentInfo);
      Info->SetModuleGuid(ModuleId);
      Key = Key_ComponentInformation;
      Key += "/" + ModuleId + "/";
      Info->SetType((const char*)Reg.GetValue(Key + Key_Type));
      Info->SetModuleName((const char*)Reg.GetValue(Key + Key_ModuleName));
      Info->SetLocation((const char*)Reg.GetValue(Key + Key_Location));
      Info->SetDescription((const char*)Reg.GetValue(Key + Key_Description));
      Registry::Key::KeysPoolPtr CalssIDs = Reg.EnumKey(Key + Key_ClassIDs)->GetChildKeys();
      for (Registry::Key::KeysPool::const_iterator i = CalssIDs->begin() ; i != CalssIDs->end() ; ++i)
        Info->AddClassId((const char*)(*i)->GetKeyValue());
      return Info;
    }

    void RegistryComponent::SetComponentInfo(const ComponentInfo &info)
    {
      std::string KeyComponentClassId = Key_ComponentClassIDs;
      KeyComponentClassId += "/";
      StringVector ClassIDs = info.GetClassIDs();
      for (StringVector::const_iterator i = ClassIDs.begin() ; i != ClassIDs.end() ; ++i)
      {
        std::string ClassId = KeyComponentClassId;
        ClassId += *i;
        Reg.CreateKey(ClassId);
        Reg.SetValue(ClassId, info.GetModuleGuid().c_str());
      }
      std::string KeyComponentInformation = Key_ComponentInformation;
      KeyComponentInformation += "/";
      KeyComponentInformation += info.GetModuleGuid();
      KeyComponentInformation += "/";
      std::string Key = KeyComponentInformation;
      Key += Key_Type;
      Reg.CreateKey(Key);
      Reg.SetValue(Key, info.GetTypeAsStr().c_str());
      Key = KeyComponentInformation;
      Key += Key_ModuleName;
      Reg.CreateKey(Key);
      Reg.SetValue(Key, info.GetModuleName().c_str());
      Key = KeyComponentInformation;
      Key += Key_Location;
      Reg.CreateKey(Key);
      Reg.SetValue(Key, info.GetLocation().c_str());
      Key = KeyComponentInformation;
      Key += Key_Description;
      Reg.CreateKey(Key);
      Reg.SetValue(Key, info.GetDescription().c_str());
      Key = KeyComponentInformation;
      Key += Key_ClassIDs;
      Reg.CreateKey(Key);
      unsigned Index = 0;
      for (StringVector::const_iterator i = ClassIDs.begin() ; i != ClassIDs.end() ; ++i)
      {
        std::stringstream Io;
        Io << Key_ClassId << Index++;
        std::string ClassIdKey = Key + "/" + Io.str();
        Reg.CreateKey(ClassIdKey);
        Reg.SetValue(ClassIdKey, i->c_str());
      }
    }
  }
}
