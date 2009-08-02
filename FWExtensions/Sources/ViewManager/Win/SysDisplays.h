#ifndef __SYSDISPLAYS_H__
#define __SYSDISPLAYS_H__

#include "../../../../Framework/Include/Exceptions.h"

#include <vector>

#include <windows.h>


DECLARE_RUNTIME_EXCEPTION(SysDisplays)

class SysDisplays
{
public:
  class SysDisplay
  {
  public:
    SysDisplay(HMONITOR display, LPCRECT rect);
    HMONITOR GetMonitor() const;
    LPCRECT GetRect() const;
  private:
    HMONITOR Display;
    RECT Rect;
  };
  typedef std::vector<SysDisplay> DisplayPool;
  SysDisplays();
  const DisplayPool& GetDisplays() const;
private:
  DisplayPool Displays;
  static BOOL CALLBACK DisplayEnumProc(HMONITOR display, HDC, LPRECT displayRect, LPARAM data);
  void AddDisplayInfo(HMONITOR display, LPCRECT rect);
};

#endif // !__SYSDISPLAYS_H__
