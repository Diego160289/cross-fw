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

bool IMainServiceImpl::OnInit()
{
  MarkToDoneService();
  return true;
}

void IMainServiceImpl::OnDone()
{
}
