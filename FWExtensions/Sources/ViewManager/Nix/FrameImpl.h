#ifndef __FRAMEIMPL_H__
#define __FRAMEIMPL_H__

#include "../../../../Framework/Include/NoCopyable.h"
#include "../../../../Framework/Include/Exceptions.h"
#include "../../../../Framework/Include/IFaces.h"
#include "../../../../Framework/Include/Pointers.h"

#include "WFExtensions.h"

#include "SysDisplays.h"

DECLARE_RUNTIME_EXCEPTION(FrameImpl)

class FrameImpl
  : private Common::NoCopyable
{
public:
  FrameImpl();
  ~FrameImpl();

  void Create(Common::SharedPtr<SysDisplays::SysDisplay> display);
  void Destroy();

  void Show(bool isVisible);
  unsigned GetWndCount() const;
  bool CreateWnd(unsigned *index, IFaces::IWndMessageHandler *handler);
  bool DestroyWnd(unsigned index);
  bool GetCurWnd(unsigned *index) const;
  bool SetCurWnd(unsigned index);


private:
};

#endif // !__FRAMEIMPL_H__
