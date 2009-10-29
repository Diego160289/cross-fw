#include "MainDataSrc.h"


IMainDataSrcImpl::IMainDataSrcImpl()
{
}

bool IMainDataSrcImpl::FinalizeCreate()
{
  return true;
}

void IMainDataSrcImpl::BeforeDestroy()
{
}

bool IMainDataSrcImpl::OnInit()
{
  return true;
}

void IMainDataSrcImpl::OnDone()
{
}
