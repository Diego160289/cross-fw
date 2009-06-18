#include <iostream>

#include "Module.h"

#include "Mutex.h"


typedef IFacesImpl::IErrorInfoImpl<IFacesTools::MultiObject, System::Mutex> ErType;

DECLARE_MODULE_ENTRY_POINT("My Module", 1234567890, TYPE_LIST_1(ErType))

int main()
{
  std::cout
    << ModuleType::GetCoClassId(0) << " "
    << std::endl;
  {
    IFacesTools::RefObjPtr<IFaces::IErrorInfo> Err(ModuleType::CreateObject(ErType::GetCoClassId()));
    std::cout << (int) IFacesTools::ModuleCounter::GetModuleCounter() << std::endl;
  }
  std::cout << (int) IFacesTools::ModuleCounter::GetModuleCounter() << std::endl;

  /*{
    IFacesTools::RefObjPtr<IFaces::IErrorInfo> Ie =
      ErType::CreateObject();
    Ie->AddError(0, "");
  }
  std::cout << (int) IFacesTools::ModuleCounter::GetModuleCounter() << std::endl;*/
  return 0;
}
