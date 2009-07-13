#ifndef __COMPONENTWRAPPERS_H__
#define __COMPONENTWRAPPERS_H__

#include"Exceptions.h"
#include "IFaces.h"
#include "RefObjPtr.h"
#include "Pointers.h"
#include "IVariantImpl.h"
#include "IEnumImpl.h"
#include "Mutex.h"
#include "Typedefs.h"

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
      typedef RefObjPtr<IFaces::IRegistryCtrl> IRegistryCtrlPtr;

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
      typedef RefObjPtr<IFaces::IRegistry> IRegistryPtr;

      class Key
      {
      public:
        typedef SharedPtr<Key> KeyPtr;
        typedef std::vector<KeyPtr> KeysPool;
        typedef SharedPtr<KeysPool> KeysPoolPtr;
        bool HasChildKeys() const;
        bool HasValue() const;
        const std::string GetKeyName() const;
        const IFacesImpl::IVariantHelper GetKeyValue() const;
        const KeysPoolPtr GetChildKeys() const;
      private:
        friend class Registry;
        mutable IFacesImpl::IEnumHelper Keys;
        std::string KeyName;
        Key(IFacesImpl::IEnumHelper keys, const std::string &keyName);
      };

      Registry(IRegistryPtr reg);
      void CreateKey(const std::string &pathKey);
      void RemoveKey(const std::string &pathKey);
      const IFacesImpl::IVariantHelper GetValue(const std::string &pathKey) const;
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
      Key::KeyPtr EnumKey(const std::string &pathKey) const;
    private:
      IRegistryPtr Reg;
    };

    class RegistryComponent
    {
    public:
      static const char Key_ComponentInformation[];
      static const char Key_ComponentClassIDs[];
      static const char Key_Type[];
      static const char Key_ModuleName[];
      static const char Key_Location[];
      static const char Key_Description[];
      static const char Key_ClassIDs[];
      static const char Key_ClassId[];

      class ComponentInfo
      {
      public:
        static const char Type_Unknown[];
        static const char Type_InProc[];
        static const char Type_OutProc[];
        static const char Type_Remote[];

        enum ComponentType { ctUnknown, ctInProc, ctOutProc, ctRemote };

        ComponentInfo();
        void SetType(ComponentType type);
        void SetType(const std::string &type);
        ComponentType GetType() const;
        const std::string GetTypeAsStr() const;
        void SetModuleGuid(const std::string &guid);
        const std::string& GetModuleGuid() const;
        void SetModuleName(const std::string &name);
        const std::string& GetModuleName() const;
        void SetLocation(const std::string &location);
        const std::string& GetLocation() const;
        void SetDescription(const std::string &description);
        const std::string& GetDescription() const;
        void AddClassId(const std::string &classId);
        const StringVector& GetClassIDs() const;
      private:
        ComponentType Type;
        std::string ModuleGuid;
        std::string ModuleName;
        std::string Location;
        std::string Description;
        StringVector ClassIDs;
      };
      typedef SharedPtr<ComponentInfo> ComponentInfoPtr;
      typedef RefObjPtr<IFaces::IRegistry> IRegistryPtr;
      RegistryComponent(IRegistryPtr reg);
      ComponentInfoPtr GetComponentInfo(const std::string &classId) const;
      void SetComponentInfo(const ComponentInfo &info);
    private:
      Registry Reg;
    };
  }
}
#endif  // !__COMPONENTWRAPPERS_H__
