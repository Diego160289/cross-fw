#ifndef __SYSDISPLAYS_H__
#define __SYSDISPLAYS_H__

#include "../../../../Framework/Include/Exceptions.h"

#include <vector>

DECLARE_RUNTIME_EXCEPTION(SysDisplays)

class SysDisplays
{
public:
  class SysDisplay
  {
  public:
  private:
  };
  typedef std::vector<SysDisplay> DisplayPool;
  SysDisplays();
  const DisplayPool& GetDisplays() const;
private:
  DisplayPool Displays;
};

#endif // !__SYSDISPLAYS_H__
