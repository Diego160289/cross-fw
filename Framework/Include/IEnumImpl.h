#ifndef __IENUMIMPL_H__
#define __IENUMIMPL_H__

#include "IFacesTools.h"
#include "SyncObj.h"
#include "Exceptions.h"

#include <vector>
#include <iterator>

namespace IFacesImpl
{
  using IFaces::RetCode;
  using IFaces::retOk;
  using IFaces::retFalse;
  using IFaces::retFail;

  template
  <
    template <typename, typename > class TCreateStrategy = Common::MultiObject,
    typename TSynObj = System::MutexStub
  >
  class IEnumImpl
    : public Common::CoClassBase
        <
          IEnumImpl<TCreateStrategy, TSynObj>,
          TYPE_LIST_1(IFaces::IEnum),
          TCreateStrategy, TSynObj
        >
  {
  public:
    DECLARE_UUID(6724ad4d-22eb-4d3d-bc0e-4283ef45373e)

    typedef IEnumImpl<TCreateStrategy, TSynObj> ThisType;
    typedef Common::RefObjPtr<ThisType> ThisTypePtr;

    IEnumImpl()
      : IsModified(false)
    {
    }
    virtual ~IEnumImpl()
    {
    }

    // IEnum
    virtual RetCode CALL_TYPE First()
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      IsModified = false;
      CurIter = Items.begin();
      return retOk;
    }
    virtual RetCode CALL_TYPE Next(IFaces::IBase **item)
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      if (IsModified)
        return retFail;
      if (CurIter == Items.end())
        return retFalse;
      if (!(*CurIter).QueryInterface(item))
        return retFail;
      ++CurIter;
      return retOk;
    }
    virtual RetCode CALL_TYPE Clone(IFaces::IEnum **newEnum) const
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      Common::RefObjPtr<ThisType> NewInst(ThisType::CreateObject());
      NewInst->Items = Items;
      if (!(NewInst->IsModified = IsModified))
      {
          NewInst->CurIter = NewInst->Items.begin() + std::distance(Items.begin(), CurIter);
      }
      return NewInst.QueryInterface(newEnum) ? retOk : retFail;
    }

    typedef Common::RefObjPtr<IFaces::IBase> IBasePtr;

    void AddItem(IBasePtr item)
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      Items.push_back(item);
      IsModified = true;
    }
    void Clear()
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      Items.clear();
      IsModified = true;
    }
  private:
    typedef std::vector<IBasePtr> IBasePtrPool;
    bool IsModified;
    mutable IBasePtrPool Items;
    typedef IBasePtrPool::iterator Iterator;
    Iterator CurIter;
  };

  template <typename TSyn>
  typename IEnumImpl<Common::MultiObject, TSyn>::ThisTypePtr
  CreateEnum()
  {
    typename IEnumImpl<Common::MultiObject, TSyn>::ThisTypePtr Ret =
      IEnumImpl<Common::MultiObject, TSyn>::CreateObject();
    return Ret;
  }

  DECLARE_RUNTIME_EXCEPTION(IEnumHelper)

  class IEnumHelper
  {
  public:
    typedef Common::RefObjPtr<IFaces::IEnum> IEnumPtr;
    typedef Common::RefObjPtr<IFaces::IBase> IBasePtr;

    IEnumHelper(IEnumPtr enumerator);
    IEnumHelper(const IEnumHelper &enumHelper);
    IEnumHelper& operator = (const IEnumHelper &enumHelper);
    IBasePtr First();
    IBasePtr Next();
  private:
    IEnumPtr Enumerator;
  };

}

#endif  // !__IENUMIMPL_H__
