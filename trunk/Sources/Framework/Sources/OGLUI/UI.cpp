#include "OGLUI/UI.h"
#include "UIImpl.h"


namespace OGLUI
{

  ClientContext::ClientContext()
    : Impl(new ClientContextImpl)
  {
  }

  ClientContext::~ClientContext()
  {
    delete Impl;
  }

}