#include "FlashView.h"


IFlashViewImpl::IFlashViewImpl()
{
}

bool IFlashViewImpl::FinalizeCreate()
{
  return true;
}

void IFlashViewImpl::BeforeDestroy()
{
}

bool IFlashViewImpl::OnInit()
{
  return true;
}

void IFlashViewImpl::OnDone()
{
}
