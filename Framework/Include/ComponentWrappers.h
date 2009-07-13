#ifndef __COMPONENTWRAPPERS_H__
#define __COMPONENTWRAPPERS_H__

#include"Exceptions.h"
#include "IFaces.h"
#include "RefObjPtr.h"
#include "Pointers.h"
#include "IVariantImpl.h"
#include "IEnumImpl.h"
#include "Mutex.h"

#include <vector>
#include <string>

namespace Common
{
  namespace Wrappers
  {

    DECLARE_RUNTIME_EXCEPTION(RegistryCtr)

    class RegistryCtrl
    {
    public:
      typedef Common::RefObjPtr<IFaces::IRegistryCtrl> IRegistryCtrlPtr;

      RegistryCtrl(IRegistryCtrlPtr regCtrl);
      void Create(const char *registryPath);
      void Remove(const char *registryPath);
      void Load(const char *registryPath);
      bool IsLoaded() const;
      void Unload();
    private:
      IRegistryCtrlPtr RegCtrl;
    };

    DECLARE_RUNTIME_EXCEPTION(Registry)

    class Registry
    {
    public:
      typedef Common::RefObjPtr<IFaces::IRegistry> IRegistryPtr;

      class Key
      {
      public:
        typedef Common::SharedPtr<Key> KeyPtr;
        typedef std::vector<KeyPtr> KeysPool;
        bool HasChildKeys() const;
        bool HasValue() const;
        const std::string GetKeyName() const;
        const IFacesImpl::IVariantHelper GetKeyValue() const;
        const KeysPool GetChildKeys() const;
      private:
        friend class Registry;
        mutable IFacesImpl::IEnumHelper Keys;
        std::string KeyName;
        Key(IFacesImpl::IEnumHelper keys, const std::string &keyName);
      };

      Registry(IRegistryPtr reg);
      void CreateKey(const std::string &pathKey);
      void RemoveKey(const std::string &pathKey);
      const IFacesImpl::IVariantHelper GetValue(const std::string &pathKey);
      template <typename T>
      void SetValue(const std::string &pathKey, const T &value)
      {
        IFacesImpl::IVariantHelper::IVariantPtr Var = IFacesImpl::CreateVariant<System::Mutex>();
        {
          IFacesImpl::IVariantHelper Helper(Var);
          Helper = value;
        }
        if (Reg->SetValue(pathKey.c_str(), Var.Get()) != IFaces::retOk)
          throw RegistryException("Can't set value");
      }
      void SetValue(const std::string &pathKey, const void *data, unsigned long bytes);
      void SetValue(const std::string &pathKey, IFaces::IBase *iface);
      Key::KeyPtr EnumKey(const std::string &pathKey);
    private:
      IRegistryPtr Reg;
    };

  }
}
#endif  // !__COMPONENTWRAPPERS_H__
