#include "ClassFactory.h"
#include "ComponentWrappers.h"

#include <iostream>

IClassFactoryImpl::IClassFactoryImpl()
{
}

RetCode IClassFactoryImpl::CreateObject(const char *classId, IFaces::IBase **obj)
{
  try
  {
    if (!classId)
      return retBadParam;
    Common::Wrappers::RegistryComponent::ComponentInfoPtr ComponentInfo;
    {
      Common::SyncObject<System::Mutex> Locker(RegistryMtx);
      if (!Registry.Get())
        return retFail;
      ComponentInfo = Common::Wrappers::RegistryComponent(Registry).GetComponentInfo(classId);
    }
    Common::SyncObject<System::Mutex> Locker(ModulesMtx);
    ModulePool::iterator ModulePair = Modules.find(ComponentInfo->GetModuleGuid());
    if (ModulePair == Modules.end())
    {
      if (ComponentInfo->GetType() == Common::Wrappers::RegistryComponent::ComponentInfo::ctInProc)
      {
        std::string ModulePath = ComponentInfo->GetLocation() + "/" + ComponentInfo->GetModuleName();
        ModuleHolderPtr NewModule(new Common::ModuleHolder(Common::ModuleHolder::DllHolderPtr(new System::DllHolder(ModulePath.c_str()))));
        if (NewModule->CreateObject(classId).QueryInterface(obj) != retOk)
          return retFail;
        Modules[ComponentInfo->GetModuleGuid()] = NewModule;
      }
      else
        return retNotImpl;
    }
    else
      return ModulePair->second->CreateObject(classId).QueryInterface(obj);
    return retOk;
  }
  catch (std::exception &)
  {
    return retFail;
  }
}

RetCode IClassFactoryImpl::SetRegistry(IFaces::IRegistry *reg)
{
  Common::SyncObject<System::Mutex> Locker(RegistryMtx);
  Registry = reg;
  return retOk;
}

RetCode IClassFactoryImpl::SetParams(IFaces::IVarMap *params)
{
  return retOk;
}

bool IClassFactoryImpl::AfterCreate()
{
  std::cout << "Create" << std::endl;
  return true;
}

void IClassFactoryImpl::BeforeDestroy()
{
  {
    Common::SyncObject<System::Mutex> Locker(RegistryMtx);
    Registry.Release();
  }
}
