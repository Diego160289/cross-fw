#include "TestService.h"


#include <iostream>


ITestServiceImpl::ITestServiceImpl()
  : Ticks(0)
{
}

RetCode ITestServiceImpl::Init(const char *instanceUUID,
                               IFaces::IClassFactory *factory,
                               IFaces::IServiceManager *manager)
{
  Manager = manager;
  try
  {
    Timer.Reset(new System::Timer(Common::CreateMemberCallback(*this, &ITestServiceImpl::OnTimer), 500));
  }
  catch (std::exception &)
  {
    return retFail;
  }
  return retOk;
}

void ITestServiceImpl::Done()
{
  Manager.Release();
}

void ITestServiceImpl::OnTimer()
{
  std::cout << Ticks++ << std::endl;
  if (Ticks == 5)
    Manager->PostStopToServiceManager();
}
