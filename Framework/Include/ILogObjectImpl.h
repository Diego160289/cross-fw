#ifndef __ILOGOBJECTIMPL_H__
#define __ILOGOBJECTIMPL_H__

#include "IFacesTools.h"
#include "SyncObj.h"
#include "Exceptions.h"


namespace IFacesImpl
{

  using IFaces::RetCode;
  using IFaces::retOk;
  using IFaces::retFalse;
  using IFaces::retFail;

  class ILogObjectImpl
    : public Common::CoClassBase
        <
          TYPE_LIST_1(IFaces::ILogObject)
        >
  {
  public:
    DECLARE_UUID(6788f442-feca-43af-a4d5-456aaa87884a)

    ILogObjectImpl()
    {
    }
    virtual ~ILogObjectImpl()
    {
    }

    // ILogObject
  };

}

#endif // !__ILOGOBJECTIMPL_H__
