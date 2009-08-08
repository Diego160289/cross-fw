#include "SysDisplays.h"


SysDisplays::SysDisplays()
{
  throw SysDisplaysException("Can't enum Displays");
}

const SysDisplays::DisplayPool& SysDisplays::GetDisplays() const
{
  return Displays;
}
