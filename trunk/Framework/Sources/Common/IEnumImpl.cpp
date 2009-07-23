#include "IEnumImpl.h"


namespace IFacesImpl
{

  IEnumImpl::IEnumImpl()
    : IsModified(false)
  {
  }

  IEnumImpl::~IEnumImpl()
  {
  }

  RetCode IEnumImpl::First()
  {
    Common::ISyncObject Locker(this->GetSynObj());
    IsModified = false;
    CurIter = Items.begin();
    return retOk;
  }

  RetCode IEnumImpl::Next(IFaces::IBase **item)
  {
    Common::ISyncObject Locker(this->GetSynObj());
    if (IsModified)
      return retFail;
    if (CurIter == Items.end())
      return retFalse;
    if ((*CurIter).QueryInterface(item) != retOk)
      return retFail;
    ++CurIter;
    return retOk;
  }

  RetCode IEnumImpl::Clone(IFaces::IEnum **newEnum) const
  {
    Common::ISyncObject Locker(GetSynObj());
    Common::RefObjPtr<IEnumImpl> NewInst =
      Common::IBaseImpl<IEnumImpl>::Create(GetSynObj());
    NewInst->Items = Items;
    if (!(NewInst->IsModified = IsModified))
    {
        NewInst->CurIter = NewInst->Items.begin() + std::distance(Items.begin(), CurIter);
    }
    return NewInst.QueryInterface(newEnum);
  }

  void IEnumImpl::AddItem(IBasePtr item)
  {
    Common::ISyncObject Locker(this->GetSynObj());
    Items.push_back(item);
    IsModified = true;
  }

  void IEnumImpl::Clear()
  {
    Common::ISyncObject Locker(this->GetSynObj());
    Items.clear();
    IsModified = true;
  }


  IEnumHelper::IEnumHelper(IEnumPtr enumerator)
    : Enumerator(enumerator)
  {
  }

  IEnumHelper::IEnumHelper(const IEnumHelper &enumHelper)
  {
    if (enumHelper.Enumerator->Clone(Enumerator.GetPPtr()) != retOk)
      throw IEnumHelperException("Can't clone enum");
  }

  IEnumHelper& IEnumHelper::operator = (const IEnumHelper &enumHelper)
  {
    IEnumHelper Tmp(enumHelper);
    Enumerator.Swap(Tmp.Enumerator);
    return *this;
  }

  IEnumHelper::IBasePtr IEnumHelper::First()
  {
    IBasePtr Ret;
    if (Enumerator->First() != retOk)
      throw IEnumHelperException("Can't get first item");
    RetCode Code = Enumerator->Next(Ret.GetPPtr());
    if (Code != retOk && Code != retFalse)
      throw IEnumHelperException("Can't get first item");
    return Ret;
  }

  IEnumHelper::IBasePtr IEnumHelper::Next()
  {
    IBasePtr Ret;
    RetCode Code = Enumerator->Next(Ret.GetPPtr());
    if (Code != retOk && Code != retFalse)
      throw IEnumHelperException("Can't get next item");
    return Ret;
  }

}
