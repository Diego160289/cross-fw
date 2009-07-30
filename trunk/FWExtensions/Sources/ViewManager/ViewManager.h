#ifndef __VIEWMANAGER_H__
#define __VIEWMANAGER_H__

#include "../../Framework/Include/IFacesTools.h"

#include "WFExtensions.h"


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

private:
};

#endif  // !__VIEWMANAGER_H__
