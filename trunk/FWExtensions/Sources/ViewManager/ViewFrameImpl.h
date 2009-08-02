#ifndef __VIEWFRAMEIMPL_H__
#define __VIEWFRAMEIMPL_H__

#include "../../Framework/Include/IFacesTools.h"

#include "WFExtensions.h"

#include "SysDisplays.h"


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

  bool FinalizeCreate();
  void BeforeDestroy();

  // IViewFrame
  virtual unsigned GetWndCount() const;
  virtual RetCode CreateWnd(unsigned *index);
  virtual RetCode DestroyWnd(unsigned index);
  virtual RetCode GetCurWndIndex(unsigned *index) const;
  virtual void* GetCurWndHandle();
  virtual RetCode SetCurWnd(unsigned index);

private:
};

#endif // !__VIEWFRAMEIMPL_H__
