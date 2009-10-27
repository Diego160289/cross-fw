#include "ClassFactory.h"
#include "ComponentWrappers.h"

#include <vector>


const unsigned IClassFactoryImpl::CleanTimeout = 5000;

IClassFactoryImpl::IClassFactoryImpl()
{
}

RetCode IClassFactoryImpl::CreateObject(const char *classId, IFaces::IBase **obj)
{
  Common::ISyncObject Locker(GetSynObj());
  try
  {
    if (!classId)
      return retBadParam;
    if (!Registry.Get())
      return retFail;
    Common::Wrappers::RegistryComponent::ComponentInfoPtr ComponentInfo =
      Common::Wrappers::RegistryComponent(Registry).GetComponentInfo(classId);
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
  Common::ISyncObject Locker(GetSynObj());
  Registry = reg;
  return retOk;
}

bool IClassFactoryImpl::FinalizeCreate()
{
  CleanLoop.Reset(new System::PulsedLoop(Common::CreateMemberCallback(*this, &IClassFactoryImpl::Clean), CleanTimeout));
  return true;
}

void IClassFactoryImpl::BeforeDestroy()
{
  Common::ISyncObject Locker(GetSynObj());
  Registry.Release();
  CleanLoop.Release();
  Clean();
}

void IClassFactoryImpl::Clean()
{
  std::vector<ModuleHolderPtr> RemovedModules;
  Common::StringVector RemovedKeys;
  {
    Common::ISyncObject Locker(GetSynObj());
    for (ModulePool::iterator i = Modules.begin() ; i != Modules.end() ; ++i)
    {
      try
      {
        if (i->second->GetModuleCounter())
          continue;
        RemovedModules.push_back(i->second);
        RemovedKeys.push_back(i->first);
      }
      catch (std::exception &)
      {
      }
    }
  }
  for (Common::StringVector::const_iterator i = RemovedKeys.begin() ; i != RemovedKeys.end() ; ++i)
    Modules.erase(*i);
}
