#include "TestService2.h"
#include "IVarMapImpl.h"
#include "RefObjQIPtr.h"


#include <iostream>


ITestService2Impl::ITestService2Impl()
  : Ticks(0)
{
}

bool ITestService2Impl::OnInit()
{
  try
  {
    /*if (!CreateObject<IFaces::IRegistry>("cf7456c3-70c7-4a97-b8e4-f910cd2f823b").Get())
      return false;*/
    Timer.Reset(new System::Timer(Common::CreateMemberCallback(*this, &ITestService2Impl::OnTimer), 500));
  }
  catch (std::exception &)
  {
    return false;
  }
  return true;
}

void ITestService2Impl::OnDone()
{
  std::cout << "ITestService2Impl::OnDone" << std::endl;
  Timer.Release();
}

void ITestService2Impl::OnTimer()
{
  std::cout << "    ITestService2Impl ----->  Tick: " << Ticks++ << std::endl;
  if (Ticks >= 15 && !CanDone())
    MarkToDoneService();
}

void ITestService2Impl::Test()
{
  std::cout << "ITestService2Impl::Test()" << std::endl;
}

bool ITestService2Impl::FinalizeCreate()
{
  std::cout << "ITestService2Impl::FinalizeCreate()" << std::endl;
  return true;
}

void ITestService2Impl::BeforeDestroy()
{
  std::cout << "ITestService2Impl::BeforeDestroy()" << std::endl;
}
