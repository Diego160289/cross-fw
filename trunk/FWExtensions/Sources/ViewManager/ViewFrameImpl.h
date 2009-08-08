#ifndef __VIEWFRAMEIMPL_H__
#define __VIEWFRAMEIMPL_H__

#include "../../Framework/Include/IFacesTools.h"
#include "../../Framework/Include/Pointers.h"

#include "WFExtensions.h"

#include "SysDisplays.h"
#include "FrameImpl.h"

using IFaces::RetCode;
using IFaces::retFail;
using IFaces::retOk;
using IFaces::retFalse;
using IFaces::retBadParam;
using IFaces::retNotImpl;

class IViewFrameImpl
  : public Common::CoClassBase
      <
        TYPE_LIST_1(IFaces::IViewFrame)
      >
{
public:
  DECLARE_UUID(57364697-b165-4c21-b66e-62a971afb57c)

  IViewFrameImpl();

  void Create(Common::SharedPtr<SysDisplays::SysDisplay> display);

  bool FinalizeCreate();
  void BeforeDestroy();

  // IViewFrame
  virtual RetCode Show(bool isVisible);
  virtual unsigned GetWndCount() const;
  virtual RetCode CreateWnd(unsigned *index, IFaces::IWndMessageHandler *handler = 0);
  virtual RetCode DestroyWnd(unsigned index);
  virtual RetCode GetCurWnd(unsigned *index) const;
  virtual RetCode SetCurWnd(unsigned index);

private:
  Common::SharedPtr<FrameImpl> Frame;
};

#endif // !__VIEWFRAMEIMPL_H__
