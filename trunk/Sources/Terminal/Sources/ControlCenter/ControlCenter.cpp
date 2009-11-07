#include "ControlCenter.h"


IControlCenterImpl::IControlCenterImpl()
{
}

bool IControlCenterImpl::FinalizeCreate()
{
  return true;
}

void IControlCenterImpl::BeforeDestroy()
{
}

bool IControlCenterImpl::OnInit()
{
  return true;
}

void IControlCenterImpl::OnDone()
{
}
