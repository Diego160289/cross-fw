#ifndef __UIIMPL_H__
#define __UIIMPL_H__


#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>


#include "OGLUI/UI.h"
#include "Pointers.h"


namespace OGLUI
{

  class ClientContext::ClientContextImpl
    : private Common::NoCopyable
  {
  public:
    ClientContextImpl(System::WindowHandle wnd, const Rect &startRect);
    ~ClientContextImpl();
    void Draw();
  private:
    HWND Wnd;
    class HDCHolder
      : private Common::NoCopyable
    {
    public:
      HDCHolder(HWND win)
        : Win(win)
        , Hdc(0)
      {
        if (!(Hdc = ::GetDC(Win)))
          throw ClientContextException("Empty window context");
      }
      ~HDCHolder()
      {
        ::ReleaseDC(Win, Hdc);
      }
      operator HDC ()
      {
        return Hdc;
      }
    private:
      HWND Win;
      HDC Hdc;
    } DC;
    class GLDC
      : private Common::NoCopyable
    {
    public:
      GLDC(HDC hdc)
        : Glrc(0)
      {
        if (!(Glrc = ::wglCreateContext(hdc)))
          throw ClientContextException("Can't create rendering context");
        if (!::wglMakeCurrent(hdc, Glrc))
        {
          ::wglDeleteContext(Glrc);
          throw ClientContextException("Can't set rendering context");
        }
      }
      ~GLDC()
      {
        ::wglMakeCurrent(NULL, NULL);
        ::wglDeleteContext(Glrc);
      }
    private:
      HGLRC Glrc;
    };
    Common::SharedPtr<GLDC> RenderingContext;
    Rect WndRect;
  };

}

#endif  // !__UIIMPL_H__
