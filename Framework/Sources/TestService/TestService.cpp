#include "TestService.h"
#include "ServiceParamNames.h"
#include "IVarMapImpl.h"
#include "RefObjQIPtr.h"


#include <iostream>


ITestServiceImpl::ITestServiceImpl()
  : Ticks(0)
{
}

void ITestServiceImpl::SetInstanceUUID(const char *instanceUUID)
{
}

RetCode ITestServiceImpl::SetParams(IFaces::IVarMap *params)
{
  if (!params)
  {
    Manager.Release();
    return retOk;
  }
  try
  {
    IFacesImpl::IVarMapHelper::IVarMapPtr VarMap(params);
    IFacesImpl::IVarMapHelper Params(VarMap);
    return Common::RefObjQIPtr<IFaces::IServiceManager>(
      (IFaces::IBase*)(Params.GetVariable(IFacesImpl::PrmServiceManager))
      ).QueryInterface(Manager.GetPPtr());
  }
  catch (std::exception &)
  {
    return retFail;
  }
  return retOk;
}

RetCode ITestServiceImpl::Init()
{
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
  if (Ticks == 50 && Manager.Get())
    Manager->PostStopToServiceManager();
}

bool ITestServiceImpl::CanDone() const
{
  return false;
}
