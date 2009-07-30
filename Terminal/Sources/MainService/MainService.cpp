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
  return true;
}

void IMainServiceImpl::OnDone()
{
}
