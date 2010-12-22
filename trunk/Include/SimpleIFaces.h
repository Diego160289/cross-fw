//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __SIMPLEIFACES_H__
#define __SIMPLEIFACES_H__

#include "Pointers.h"
#include "Variant.h"
#include "Exceptions.h"
#include "NamedVarMap.h"

#include <map>
#include <string>


namespace Common
{
  struct ICallback
  {
    virtual ~ICallback()
    {
    }
    virtual void Do() = 0;
  };

  typedef Common::SharedPtr<ICallback> ICallbackPtr;

  template <typename TObject>
  class IMemberCallbackImpl
    : public ICallback
  {
  public:
    typedef void (TObject::*PMethod)();
    IMemberCallbackImpl(TObject &obj, PMethod mtd)
      : Obj(obj)
      , Mtd(mtd)
    {
    }
    virtual void Do()
    {
      (Obj.*Mtd)();
    }
  private:
    TObject &Obj;
    PMethod Mtd;
  };

  template <typename TObject>
  ICallbackPtr CreateMemberCallback(TObject &obj, void (TObject::*mtd)())
  {
    return ICallbackPtr(new IMemberCallbackImpl<TObject>(obj, mtd));
  }

  template <typename TPFunc>
  class IFuncCallbackImpl
    : public ICallback
  {
  public:
    IFuncCallbackImpl(TPFunc *func)
      : Func(func)
    {
    }
    virtual void Do()
    {
      Func();
    }
  private:
    TPFunc *Func;;
  };

  template <typename TFunc>
  ICallbackPtr CreateFuncCallback(TFunc *func)
  {
    return ICallbackPtr(new IFuncCallbackImpl<TFunc>(func));
  }

  typedef NamedVarMap<std::wstring> VarMap;
  typedef SharedPtr<VarMap> VarMapPtr;

  DECLARE_RUNTIME_EXCEPTION(GetParamFromVarMap)

  struct ICallbackEx
  {
    virtual ~ICallbackEx()
    {
    }
    virtual VarMapPtr Do(const VarMap &varMap) = 0;
  };

  typedef Common::SharedPtr<ICallbackEx> ICallbackExPtr;

  template <typename TObject>
  class IMemberCallbackExImpl
    : public ICallbackEx
  {
  public:
    typedef VarMapPtr (TObject::*PMethod)(const VarMap &);
    IMemberCallbackExImpl(TObject &obj, PMethod mtd)
      : Obj(obj)
      , Mtd(mtd)
    {
    }
    virtual VarMapPtr Do(const VarMap &varMap)
    {
      return (Obj.*Mtd)(varMap);
    }
  private:
    TObject &Obj;
    PMethod Mtd;
  };

  template <typename TObject>
  ICallbackExPtr CreateMemberCallbackEx(TObject &obj,
    typename IMemberCallbackExImpl<TObject>::PMethod mtd)
  {
    return ICallbackExPtr(new IMemberCallbackExImpl<TObject>(obj, mtd));
  }

}

#endif  // !__SIMPLEIFACES_H__
