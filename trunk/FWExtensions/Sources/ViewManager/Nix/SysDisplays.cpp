#include "SysDisplays.h"


SysDisplays::SysDisplay::SysDisplay(Display *display, int width, int height)
  : Display_(display)
  , Width(width)
  , Height(height)
{
}

Display* SysDisplays::SysDisplay::GetDisplay() const
{
  return Display_;
}

int SysDisplays::SysDisplay::GetWidth() const
{
  return Width;
}

int SysDisplays::SysDisplay::GetHeight() const
{
  return Height;
}


SysDisplays::SysDisplays()
{
  _XPrivDisplay DefDisplay = reinterpret_cast<_XPrivDisplay>(XOpenDisplay(0));
  if (!DefDisplay)
    throw SysDisplaysException("Can't open default display");
  for (int i = 0 ; i < DefDisplay->nscreens ; ++i)
  {
    Displays.push_back(SysDisplay(DefDisplay->screens[i].display,
        DefDisplay->screens[i].width, DefDisplay->screens[i].height));
  }
}

const SysDisplays::DisplayPool& SysDisplays::GetDisplays() const
{
  return Displays;
}
