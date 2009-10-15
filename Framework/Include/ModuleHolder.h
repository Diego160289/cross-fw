#ifndef __MODULEHOLDER_H__
#define __MODULEHOLDER_H__

#include <string>

#include "DllHolder.h"
#include "Pointers.h"
#include "IFaces.h"
#include "RefObjPtr.h"
#include "RefObjQIPtr.h"
#include "Typedefs.h"

namespace Common
{
  DECLARE_RUNTIME_EXCEPTION(ModuleHolder)

  class ModuleHolder
    : private NoCopyable
  {
  public:
    typedef SharedPtr<System::DllHolder> DllHolderPtr;
    typedef RefObjPtr<IFaces::IBase> IBasePtr;
    ModuleHolder(DllHolderPtr dll);
    const std::string& GetModuleGuid() const;
    const std::string& GetModuleName() const;
    const Common::StringVector& GetCoClassIds() const;
    IBasePtr CreateObject(const char *classId);
    unsigned long GetModuleCounter() const;
  private:
    mutable DllHolderPtr Dll;
    mutable std::string ModuleName;
    mutable std::string ModuleGuid;
    mutable Common::StringVector CoClassIds;
  };

}

#endif  // !__MODULEHOLDER_H__
