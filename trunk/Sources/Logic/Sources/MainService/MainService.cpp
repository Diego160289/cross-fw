#include "MainService.h"


IMainServiceImpl::IMainServiceImpl()
{
}

bool IMainServiceImpl::FinalizeCreate()
{
  return true;
}

void IMainServiceImpl::BeforeDestroy()
{
}

#include <conio.h>
#include <iostream>

bool IMainServiceImpl::OnInit()
{
  try
  {
    DataSrc = StartService("e1156d71-fd1a-4a4a-82ed-a1ab73dc9c87");
    View = StartService("ac0dd189-8db2-47fb-a590-438fd151910a");
    View->SetCallback(this);
    View->ShowView();
  }
  catch (std::exception &)
  {
    return false;
  }
  return true;
}

void IMainServiceImpl::OnDone()
{
  DataSrc.Release();
}

RetCode IMainServiceImpl::OnQueryExternalResource(const char *resName, IFaces::IStream **stream)
{
  return !DataSrc.Get() ? retFail : DataSrc->GetResource(resName, stream);
}

RetCode IMainServiceImpl::GetBusinessCategories(const wchar_t *callback, const wchar_t *serviceId,
                                                const wchar_t *method, const wchar_t *objectId,
                                                const wchar_t *frameId)
{
  return retFail;
}
