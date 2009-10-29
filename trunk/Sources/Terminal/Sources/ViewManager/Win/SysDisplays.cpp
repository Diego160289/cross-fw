#include "SysDisplays.h"


SysDisplays::SysDisplay::SysDisplay(HMONITOR display, LPCRECT rect)
  : Display(display)
{
  ::CopyMemory(&Rect, rect, sizeof(Rect));
}

HMONITOR SysDisplays::SysDisplay::GetMonitor() const
{
  return Display;
}

LPCRECT SysDisplays::SysDisplay::GetRect() const
{
  return &Rect;
}


SysDisplays::SysDisplays()
{
  if (!EnumDisplayMonitors(0, 0, &SysDisplays::DisplayEnumProc, reinterpret_cast<LPARAM>(this)))
    throw SysDisplaysException("Can't enum Displays");
}

const SysDisplays::DisplayPool& SysDisplays::GetDisplays() const
{
  return Displays;
}

BOOL CALLBACK SysDisplays::DisplayEnumProc(HMONITOR display, HDC, LPRECT displayRect, LPARAM data)
{
  reinterpret_cast<SysDisplays*>(data)->AddDisplayInfo(display, displayRect);
  return TRUE;
}

void SysDisplays::AddDisplayInfo(HMONITOR display, LPCRECT rect)
{
  Displays.push_back(SysDisplay(display, rect));
}
