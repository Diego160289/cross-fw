#include "TestService.h"
#include "IVarMapImpl.h"
#include "RefObjQIPtr.h"


#include <iostream>


ITestServiceImpl::ITestServiceImpl()
  : Ticks(0)
{
}

bool ITestServiceImpl::OnInit()
{
  try
  {
    if (!CreateObject<IFaces::IRegistry>("cf7456c3-70c7-4a97-b8e4-f910cd2f823b").Get())
      return false;
    Timer.Reset(new System::Timer(Common::CreateMemberCallback(*this, &ITestServiceImpl::OnTimer), 500));
  }
  catch (std::exception &)
  {
    return false;
  }
  return true;
}

void ITestServiceImpl::OnTimer()
{
  std::cout << "Tick: " << Ticks++ << std::endl;
  if (Ticks == 5)
    PostStopToServiceManager();
}
