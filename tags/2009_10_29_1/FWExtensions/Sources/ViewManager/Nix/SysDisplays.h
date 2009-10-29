#ifndef __SYSDISPLAYS_H__
#define __SYSDISPLAYS_H__

#include "../../../../Framework/Include/Exceptions.h"

#include <vector>

#include <X11/Xlib.h>


DECLARE_RUNTIME_EXCEPTION(SysDisplays)

class SysDisplays
{
public:
  class SysDisplay
  {
  public:
    SysDisplay(Display *display, int width, int height);
    Display* GetDisplay() const;
    int GetWidth() const;
    int GetHeight() const;
  private:
    Display *Display_;
    int Width;
    int Height;
  };
  typedef std::vector<SysDisplay> DisplayPool;
  SysDisplays();
  const DisplayPool& GetDisplays() const;

private:
  DisplayPool Displays;
};

#endif // !__SYSDISPLAYS_H__
