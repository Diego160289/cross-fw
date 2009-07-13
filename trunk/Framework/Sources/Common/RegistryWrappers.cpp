#include "ComponentWrappers.h"
#include "RefObjQIPtr.h"

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

    const IFacesImpl::IVariantHelper Registry::GetValue(const std::string &pathKey)
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

    Registry::Key::KeyPtr Registry::EnumKey(const std::string &pathKey)
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
      Common::RefObjQIPtr<IFaces::INamedVariable> Var = Keys.First();
      if (!Var.Get())
        throw RegistryException("Can't get variable");
      Common::RefObjPtr<IFaces::IVariant> Value;
      if (Var->Get(Value.GetPPtr()) != IFaces::retOk)
        throw RegistryException("Can't get value");
      if (Value->GetType() != IFaces::IVariant::vtIBase)
        return false;
      Common::RefObjPtr<IFaces::IBase> ChildItem;
      if (Value->GetValue(reinterpret_cast<void**>(ChildItem.GetPPtr())) != IFaces::retOk ||
        !Common::RefObjQIPtr<IFaces::IEnum>(ChildItem).Get())
      {
        throw RegistryException("Unknown child item");
      }
      return true;
    }

    bool Registry::Key::HasValue() const
    {
      Common::RefObjQIPtr<IFaces::INamedVariable> Var = Keys.First();
      if (!Var.Get())
        throw RegistryException("Can't get variable");
      Common::RefObjPtr<IFaces::IVariant> Value;
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
      Common::RefObjQIPtr<IFaces::INamedVariable> Var = Keys.First();
      if (!Var.Get())
        throw RegistryException("Can't get variable");
      Common::RefObjPtr<IFaces::IVariant> Value;
      if (Var->Get(Value.GetPPtr()) != IFaces::retOk)
        throw RegistryException("Can't get value");
      if (Value->GetType() == IFaces::IVariant::vtIBase)
        throw RegistryException("Is not value");
      return Value;
    }

    const Registry::Key::KeysPool Registry::Key::GetChildKeys() const
    {
      KeysPool RetPool;
      for (Common::RefObjQIPtr<IFaces::INamedVariable> Var = Keys.First() ;
        Var.Get() ; Var = Keys.Next())
      {
        Common::RefObjPtr<IFaces::IVariant> Value;
        if (Var->Get(Value.GetPPtr()) != IFaces::retOk)
          throw RegistryException("Can't get value");
        if (Value->GetType() != IFaces::IVariant::vtIBase)
          throw RegistryException("Unknown child item");
        Common::RefObjPtr<IFaces::IBase> ChildItem;
        if (Value->GetValue(reinterpret_cast<void**>(ChildItem.GetPPtr())) != IFaces::retOk)
          throw RegistryException("Can't get child enum");
        Common::RefObjQIPtr<IFaces::IEnum> Enum(ChildItem);
        if (!Enum.Get())
          throw RegistryException("Unknown child item");
        RetPool.push_back(KeyPtr(new Key(IFacesImpl::IEnumHelper(Enum), Var->GetName())));
      }
      return RetPool;
    }

  }
}
