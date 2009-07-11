#ifndef __MESSAGEQUEUE_H__
#define __MESSAGEQUEUE_H__

#include "IFacesTools.h"
#include "Mutex.h"

using IFaces::RetCode;
using IFaces::retFail;
using IFaces::retOk;
using IFaces::retFalse;

class IClassFactoryImpl
  : public Common::CoClassBase
      <
        IClassFactoryImpl,
        TYPE_LIST_2(IFaces::IClassFactory, IFaces::IClassFactoryCtrl),
        Common::MultiObject,
        System::Mutex
      >
{
public:
  DECLARE_UUID(0eedde75-ce15-4eba-9026-3d5f94488c26)

  IClassFactoryImpl();

  // IClassFactory
  virtual RetCode CreateObject(const char *classId, IFaces::IBase **obj);

  // IClassFactoryCtrl
  virtual RetCode SetRegistry(IFaces::IRegistry *reg) ;
  virtual RetCode SetParams(IFaces::IVarMap *params);

  virtual bool AfterCreate();
  virtual void BeforeDestroy();

private:
};

#endif  // !__MESSAGEQUEUE_H__
