//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


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
    Common::ISyncObject Locker(GetSynObj());
    IsModified = false;
    CurIter = Items.begin();
    return retOk;
  }

  RetCode IEnumImpl::Next(IFaces::IBase **item)
  {
    Common::ISyncObject Locker(GetSynObj());
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
      Common::IBaseImpl<IEnumImpl>::CreateWithSyn(GetSynObj());
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
    if (!item.Get())
      throw IEnumHelperException("Empty item");
    Items.push_back(item);
    IsModified = true;
  }

  void IEnumImpl::Clear()
  {
    Common::ISyncObject Locker(this->GetSynObj());
    Items.clear();
    IsModified = true;
  }

  Common::RefObjPtr<IEnumImpl>
  CreateEnum(const Common::ISynObj &syn)
  {
    return Common::IBaseImpl<IEnumImpl>::CreateWithSyn(syn);
  }



  IEnumHelper::IEnumHelper(IEnumPtr enumerator)
    : Enumerator(enumerator)
  {
    if (!Enumerator.Get())
      IEnumHelperException("Empty IEnum pointer");
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
    if (!Enumerator.Get())
      throw IEnumHelperException("Empty enum");
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
