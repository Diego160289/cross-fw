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

  template
  <
    typename TSynObj = System::MutexStub
  >
  class ILogObjectImpl
    : public Common::CoClassBase
        <
          ILogObjectImpl<TSynObj>,
          TYPE_LIST_1(IFaces::ILogObject),
          Common::MultiObject, TSynObj
        >
  {
  public:
    DECLARE_UUID(6788f442-feca-43af-a4d5-456aaa87884a)

    typedef ILogObjectImpl<TSynObj> ThisType;
    typedef Common::RefObjPtr<ThisType> ThisTypePtr;

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
