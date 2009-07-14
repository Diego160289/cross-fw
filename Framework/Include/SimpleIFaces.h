#ifndef __SIMPLEIFACES_H__
#define __SIMPLEIFACES_H__

#include "Pointers.h"


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
}

#endif  // !__SIMPLEIFACES_H__
