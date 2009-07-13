#ifndef __SIMPLEIFACES_H__
#define __SIMPLEIFACES_H__

namespace Common
{
  struct ICallback
  {
    virtual ~ICallback()
    {
    }
    virtual void Do() = 0;
  };

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

}

#endif  // !__SIMPLEIFACES_H__
