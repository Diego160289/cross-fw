#ifndef __UI_H__
#define __UI_H__

#include "../NoCopyable.h"
#include "../Exceptions.h"

#include "UITypes.h"
#include "../SystemTypes.h"

namespace OGLUI
{

  DECLARE_RUNTIME_EXCEPTION(ClientContext)

  class ClientContext
    : private Common::NoCopyable
  {
  public:
    ClientContext(System::WindowHandle wnd, const Rect &startRect);
    ~ClientContext();
    void Draw();
  private:
    class ClientContextImpl;
    ClientContextImpl *Impl;
  };

}

#endif  // !__UI_H__
