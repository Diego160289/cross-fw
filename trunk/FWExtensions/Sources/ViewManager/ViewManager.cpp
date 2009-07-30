#include "ViewManager.h"


IViewManagerImpl::IViewManagerImpl()
{
}

bool IViewManagerImpl::FinalizeCreate()
{
  return true;
}

void IViewManagerImpl::BeforeDestroy()
{
}
