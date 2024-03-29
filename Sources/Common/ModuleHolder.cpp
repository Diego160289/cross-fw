//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "ModuleHolder.h"


namespace Common
{
  ModuleHolder::ModuleHolder(DllHolderPtr dll)
    : Dll(dll)
  {
    if (!Dll.Get())
      throw ModuleHolderException("Dll is empty");
  }

  const std::string& ModuleHolder::GetModuleGuid() const
  {
    if (ModuleGuid.empty())
    {
      typedef const char* (*PFNGetModuleGuid)();
      ModuleGuid = Dll->GetProc<PFNGetModuleGuid>("GetModuleGuid")();
    }
    return ModuleGuid;
  }

  const std::string& ModuleHolder::GetModuleName() const
  {
    if (ModuleName.empty())
    {
      typedef const char* (*PFNGetModuleName)();
      ModuleName = Dll->GetProc<PFNGetModuleName>("GetModuleName")();
    }
    return ModuleName;
  }

  const Common::StringVector& ModuleHolder::GetCoClassIds() const
  {
    if (CoClassIds.empty())
    {
      typedef unsigned (*PFNGetCoClassCount)();
      typedef const char* (*PFNGetCoClassId)(unsigned);
      unsigned Count = Dll->GetProc<PFNGetCoClassCount>("GetCoClassCount")();
      for (unsigned i = 0 ; i < Count ; ++i)
        CoClassIds.push_back(Dll->GetProc<PFNGetCoClassId>("GetCoClassId")(i));
    }
    return CoClassIds;
  }

  ModuleHolder::IBasePtr ModuleHolder::CreateObject(const char *classId)
  {
    typedef bool (*PFNCreateObject)(const char *, void **);
    IBasePtr Ret;
    if (!Dll->GetProc<PFNCreateObject>("CreateObject")(classId, reinterpret_cast<void**>(Ret.GetPPtr())))
      throw ModuleHolderException("Can't create object");
    return IBasePtr(Ret);
  }

  unsigned long ModuleHolder::GetModuleCounter() const
  {
    typedef unsigned long (*PFNGetModuleCounter)();
    return Dll->GetProc<PFNGetModuleCounter>("GetModuleCounter")();
  }
}
