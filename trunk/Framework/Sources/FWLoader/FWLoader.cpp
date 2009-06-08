#include <iostream>

#include "IErrorInfoImpl.h"

#include "Mutex.h"

int main()
{
  {
    IFacesTools::RefObjPtr<IFaces::IErrorInfo> Ie = IFacesImpl::IErrorInfoImpl<IFacesTools::MultiObject, System::Mutex>::CreateObject();
    Ie->AddError(0, "");
  }
  std::cout << (int) IFacesTools::ModuleCounter::GetModuleCounter() << std::endl;
  return 0;
}
