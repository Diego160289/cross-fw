#include "IEnumImpl.h"

namespace IFacesImpl
{
  IEnumHelper::IEnumHelper(IEnumPtr enumerator)
    : Enumerator(enumerator)
  {
  }

  IEnumHelper::IEnumHelper(const IEnumHelper &enumHelper)
  {
    if (enumHelper.Enumerator->Clone(&Enumerator) != retOk)
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
    RetCode Code = Enumerator->Next(&Ret);
    if (Code != retOk && Code != retFalse)
      throw IEnumHelperException("Can't get first item");
    return Ret;
  }

  IEnumHelper::IBasePtr IEnumHelper::Next()
  {
    IBasePtr Ret;
    RetCode Code = Enumerator->Next(&Ret);
    if (Code != retOk && Code != retFalse)
      throw IEnumHelperException("Can't get first item");
    return Ret;
  }

}
