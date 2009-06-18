#include <iostream>

#include "Module.h"

#include "Mutex.h"


typedef IFacesImpl::IErrorInfoImpl<Common::MultiObject, System::Mutex> ErType;

DECLARE_MODULE_ENTRY_POINT("My Module", 1234567890, TYPE_LIST_1(ErType))

int main()
{
  std::cout
    << ModuleType::GetCoClassId(0) << " "
    << std::endl;
  {
    Common::RefObjPtr<IFaces::IErrorInfo> Err(ModuleType::CreateObject(ErType::GetCoClassId()));
    std::cout << (int) Common::ModuleCounter::GetModuleCounter() << std::endl;
  }
  std::cout << (int) Common::ModuleCounter::GetModuleCounter() << std::endl;
  return 0;
}
