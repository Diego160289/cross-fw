#include "TestService.h"


#include <iostream>


ITestServiceImpl::ITestServiceImpl()
  : Ticks(0)
{
}

RetCode ITestServiceImpl::Init() 
{
  try
  {
    Timer.Reset(new System::Timer(Common::CreateMemberCallback(*this, &ITestServiceImpl::OnTimer), 1000));
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

RetCode ITestServiceImpl::SetClassFactory(IFaces::IClassFactory *factory)
{
  return retOk;
}

RetCode ITestServiceImpl::SetServiceManager(IFaces::IServiceManager *manager)
{
  Manager = manager;
  return retOk;
}

void ITestServiceImpl::OnTimer()
{
  std::cout << Ticks++ << std::endl;
  if (Ticks == 5)
    Manager->PostStopToServiceManager();
}
