//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __CLASSFACTORY_H__
#define __CLASSFACTORY_H__

#include "IFacesTools.h"
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
        TYPE_LIST_2(IFaces::IClassFactory, IFaces::IClassFactoryCtrl)
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
  Common::RefObjPtr<IFaces::IRegistry> Registry;

  static const unsigned CleanTimeout;
  Common::SharedPtr<System::PulsedLoop> CleanLoop;
  void Clean();

  typedef Common::SharedPtr<Common::ModuleHolder> ModuleHolderPtr;
  typedef std::map<std::string, ModuleHolderPtr> ModulePool;

  ModulePool Modules;
};

#endif  // !__CLASSFACTORY_H__
