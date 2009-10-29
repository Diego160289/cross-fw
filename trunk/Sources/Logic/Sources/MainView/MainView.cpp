#include "MainView.h"


IMainViewImpl::IMainViewImpl()
{
}

bool IMainViewImpl::FinalizeCreate()
{
  return true;
}

void IMainViewImpl::BeforeDestroy()
{
}
bool IMainViewImpl::OnInit()
{
  return true;
}

void IMainViewImpl::OnDone()
{
}
