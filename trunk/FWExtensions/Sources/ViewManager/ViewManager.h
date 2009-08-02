#ifndef __VIEWMANAGER_H__
#define __VIEWMANAGER_H__

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


class IViewManagerImpl
  : public Common::CoClassBase
      <
        TYPE_LIST_1(IFaces::IViewManager)
      >
{
public:
  DECLARE_UUID(dc5597fe-e0ed-4f60-a288-7771b947274c)

  IViewManagerImpl();

  bool FinalizeCreate();
  void BeforeDestroy();

  // IViewManager
  virtual unsigned GetDisplayCount() const;
  virtual RetCode GetDisplay(unsigned index, IFaces::IDisplay **display);

private:
  Common::AutoPtr<SysDisplays> Displays;
};

#endif  // !__VIEWMANAGER_H__
