#ifndef __MESSAGEQUEUE_H__
#define __MESSAGEQUEUE_H__

#include "IFacesTools.h"
#include "Mutex.h"
#include "Pointers.h"
#include "RefObjPtr.h"
#include "ModuleHolder.h"
#include "PulsedLoop.h"

#include <map>
#include <string>

using IFaces::RetCode;
using IFaces::retFail;
using IFaces::retOk;
using IFaces::retFalse;
using IFaces::retBadParam;
using IFaces::retNotImpl;

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

  virtual bool FinalizeCreate();
  virtual void BeforeDestroy();

private:
  System::Mutex RegistryMtx;
  Common::RefObjPtr<IFaces::IRegistry> Registry;

  static const unsigned CleanTimeout;
  Common::SharedPtr<System::PulsedLoop> CleanLoop;
  void Clean();

public:
  typedef Common::SharedPtr<Common::ModuleHolder> ModuleHolderPtr;
  typedef std::map<std::string, ModuleHolderPtr> ModulePool;

private:
  System::Mutex ModulesMtx;
  ModulePool Modules;
};

#endif  // !__MESSAGEQUEUE_H__
