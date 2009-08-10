#include "IStorageHelper.h"


namespace IFacesImpl
{

  ISorageHelper::ISorageHelper(IStoragePtr storage)
    : Storage(storage)
  {
    if (!Storage.Get())
      throw ISorageHelperException("Empty storage pointer");
  }

  ISorageHelper::IStoragePtr ISorageHelper::CreateStorage(const std::string &name)
  {
    IStoragePtr Ret;
    if (Storage->CreateStorage(name.c_str(), Ret.GetPPtr()) != retOk)
      throw ISorageHelperException("Can't create storage");
    return Ret;
  }

  ISorageHelper::IStoragePtr ISorageHelper::OpenStorage(const std::string &name)
  {
    IStoragePtr Ret;
    if (Storage->OpenStorage(name.c_str(), Ret.GetPPtr()) != retOk)
      throw ISorageHelperException("Can't open storage");
    return Ret;
  }

  ISorageHelper::IStreamPtr ISorageHelper::CreateStream(const std::string &name)
  {
    IStreamPtr Ret;
    if (Storage->CreateStream(name.c_str(), Ret.GetPPtr()) != retOk)
      throw ISorageHelperException("Can't create stream");
    return Ret;
  }

  ISorageHelper::IStreamPtr ISorageHelper::OpenStream(const std::string &name)
  {
    IStreamPtr Ret;
    if (Storage->OpenStream(name.c_str(), Ret.GetPPtr()) != retOk)
      throw ISorageHelperException("Can't open stream");
    return Ret;
  }

  void ISorageHelper::RemoveItem(const std::string &name)
  {
    if (Storage->RemoveItem(name.c_str()) != retOk)
      throw ISorageHelperException("Can't remove storage item");
  }

  IEnumHelper ISorageHelper::Enum() const
  {
    IEnumHelper::IEnumPtr Ret;
    if (Storage->Enum(Ret.GetPPtr()) != retOk)
      throw ISorageHelperException("Can't enum storage");
    return Ret;
  }

}
