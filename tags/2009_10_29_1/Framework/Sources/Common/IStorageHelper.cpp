#include "IStorageHelper.h"


namespace IFacesImpl
{

  IStorageHelper::IStorageHelper(IStoragePtr storage)
    : Storage(storage)
  {
    if (!Storage.Get())
      throw IStorageHelperException("Empty storage pointer");
  }

  IStorageHelper::IStoragePtr IStorageHelper::CreateStorage(const std::string &name)
  {
    IStoragePtr Ret;
    if (Storage->CreateStorage(name.c_str(), Ret.GetPPtr()) != retOk)
      throw IStorageHelperException("Can't create storage");
    return Ret;
  }

  IStorageHelper::IStoragePtr IStorageHelper::OpenStorage(const std::string &name)
  {
    IStoragePtr Ret;
    if (Storage->OpenStorage(name.c_str(), Ret.GetPPtr()) != retOk)
      throw IStorageHelperException("Can't open storage");
    return Ret;
  }

  IStorageHelper::IStreamPtr IStorageHelper::CreateStream(const std::string &name)
  {
    IStreamPtr Ret;
    if (Storage->CreateStream(name.c_str(), Ret.GetPPtr()) != retOk)
      throw IStorageHelperException("Can't create stream");
    return Ret;
  }

  IStorageHelper::IStreamPtr IStorageHelper::OpenStream(const std::string &name)
  {
    IStreamPtr Ret;
    if (Storage->OpenStream(name.c_str(), Ret.GetPPtr()) != retOk)
      throw IStorageHelperException("Can't open stream");
    return Ret;
  }

  void IStorageHelper::RemoveItem(const std::string &name)
  {
    if (Storage->RemoveItem(name.c_str()) != retOk)
      throw IStorageHelperException("Can't remove storage item");
  }

  IEnumHelper IStorageHelper::Enum() const
  {
    IEnumHelper::IEnumPtr Ret;
    if (Storage->Enum(Ret.GetPPtr()) != retOk)
      throw IStorageHelperException("Can't enum storage");
    return Ret;
  }

}
