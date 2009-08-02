#ifndef __DISPLAYIMPL_H__
#define __DISPLAYIMPL_H__

#include "../../Framework/Include/IFacesTools.h"
#include "../../Framework/Include/Pointers.h"

#include "WFExtensions.h"

#include "SysDisplays.h"


using IFaces::RetCode;
using IFaces::retFail;
using IFaces::retOk;
using IFaces::retFalse;
using IFaces::retBadParam;
using IFaces::retNotImpl;

class IDisplayImpl
  : public Common::CoClassBase
      <
        TYPE_LIST_1(IFaces::IDisplay)
      >
{
public:
  DECLARE_UUID(0d923cbb-cc30-48aa-852a-6f8c9ebf60c9)

  typedef Common::SharedPtr<SysDisplays::SysDisplay> SysDisplayPtr;
  IDisplayImpl();

  void SetDisplay(SysDisplayPtr display);

  bool FinalizeCreate();
  void BeforeDestroy();

  // IDisplay
  virtual bool ExistsFrame() const;
  virtual RetCode CreateFrame(IFaces::IViewFrame **frame);
  virtual RetCode GetFrame(IFaces::IViewFrame **frame);

private:
  Common::SharedPtr<SysDisplays::SysDisplay> Display;
};

#endif // !__DISPLAYIMPL_H__
