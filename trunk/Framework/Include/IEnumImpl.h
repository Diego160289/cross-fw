#ifndef __IENUMIMPL_H__
#define __IENUMIMPL_H__

#include "IFacesTools.h"
#include "SyncObj.h"
#include "Exceptions.h"

#include <list>
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

    IEnumImpl()
      : IsModified(false)
    {
    }
    virtual ~IEnumImpl()
    {
    }

    // IEnum
    virtual RetCode First()
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      IsModified = false;
      CurIter = Items.begin();
      return retOk;
    }
    virtual RetCode Next(IFaces::IBase **item)
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      if (IsModified)
        return retFail;
      if (CurIter == Items.end())
        return retFalse;
      if (!(*CurIter)->QueryInterface(item))
        return retFail;
      ++CurIter;
      return retOk;
    }
    virtual RetCode Clone(IFaces::IEnum **newEnum) const
    {
      Common::SyncObject<TSynObj> Locker(this->GetSynObj());
      Common::RefObjPtr<ThisType> NewInst(ThisType::CreateObject());
      NewInst->Items = Items;
      if (!(NewInst->IsModified = IsModified))
      {
          NewInst->CurIter = NewInst->Items.begin() +
            std::distance(Items.begin(), CurIter);
      }
      return NewInst->QueryInterface(newEnum);
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
    typedef std::list<IBasePtr> IBasePtrPool;
    bool IsModified;
    IBasePtrPool Items;
    typedef IBasePtrPool::iterator Iterator;
    Iterator CurIter;
  };

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
