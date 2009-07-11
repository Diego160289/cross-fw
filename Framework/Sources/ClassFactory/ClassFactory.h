#ifndef __MESSAGEQUEUE_H__
#define __MESSAGEQUEUE_H__

#include "IFacesTools.h"
#include "Mutex.h"

class IClassFactoryImpl
  : public Common::CoClassBase
      <
        IClassFactoryImpl,
        TYPE_LIST_1(IFaces::IClassFactory),
        Common::MultiObject,
        System::Mutex
      >
{
public:
  DECLARE_UUID(0eedde75-ce15-4eba-9026-3d5f94488c26)

  IClassFactoryImpl();

  // IClassFactory

private:
};

#endif  // !__MESSAGEQUEUE_H__
