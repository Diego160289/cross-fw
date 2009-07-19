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
