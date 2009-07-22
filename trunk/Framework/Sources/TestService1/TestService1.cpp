#include "TestService1.h"
#include "IVarMapImpl.h"
#include "RefObjQIPtr.h"


#include <iostream>


ITestService1Impl::ITestService1Impl()
  : Ticks(0)
{
}

bool ITestService1Impl::OnInit()
{
  try
  {
    Timer.Reset(new System::Timer(Common::CreateMemberCallback(*this, &ITestService1Impl::OnTimer), 800));
  }
  catch (std::exception &)
  {
    return false;
  }
  return true;
}

void ITestService1Impl::OnDone()
{
  std::cout << "ITestService1Impl::OnDone" << std::endl;
  Timer.Release();
}

void ITestService1Impl::OnTimer()
{
  std::cout << "  ITestService1Impl ----->  Tick: " << Ticks++ << std::endl;
  if (Ticks >= 25 && !CanDone())
    MarkToDoneService();
}

void ITestService1Impl::Test()
{
  std::cout << "ITestService1Impl::Test()" << std::endl;
}

bool ITestService1Impl::FinalizeCreate()
{
  std::cout << "ITestService1Impl::FinalizeCreate()" << std::endl;
  return true;
}

void ITestService1Impl::BeforeDestroy()
{
  std::cout << "ITestService1Impl::BeforeDestroy()" << std::endl;
}
