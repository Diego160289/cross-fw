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
    /*if (!CreateObject<IFaces::IRegistry>("cf7456c3-70c7-4a97-b8e4-f910cd2f823b").Get())
      return false;*/
    Timer.Reset(new System::Timer(Common::CreateMemberCallback(*this, &ITestServiceImpl::OnTimer), 500));
  }
  catch (std::exception &)
  {
    return false;
  }
  return true;
}

void ITestServiceImpl::OnDone()
{
  std::cout << "Done ITestServiceImpl" << std::endl;
  std::cout << "ITestServiceImpl::OnDone" << std::endl;
  Timer.Release();
}

void ITestServiceImpl::OnTimer()
{
  Common::RefObjQIPtr<IFaces::ILogObject> Log(GetThisIBase());
  std::cout << "ITestServiceImpl ----->  Tick: " << Ticks++ << std::endl;
  if (Ticks >= 10 && !CanDone())
    MarkToDoneService();

  if (Ticks == 5)
  {
    std::cout
      << (GetServiceManager()->StartService("725d19bb-e6bc-4a42-986d-721f0fb1c5d6") ? "TestService1 started" : "TestService1 can't started")
      << std::endl;
  }
  if (Ticks == 7)
  {
    std::cout
      << (GetServiceManager()->StartService("12457697-5839-4bb4-926a-8b5e37abcd6c") ? "TestService2 started" : "TestService1 can't started")
      << std::endl;
  }
}

void ITestServiceImpl::Test()
{
  std::cout << "ITestServiceImpl::Test()" << std::endl;
}

bool ITestServiceImpl::FinalizeCreate()
{
  std::cout << "ITestServiceImpl::FinalizeCreate()" << std::endl;
  return true;
}

void ITestServiceImpl::BeforeDestroy()
{
  std::cout << "ITestServiceImpl::BeforeDestroy()" << std::endl;
}
