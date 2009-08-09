#include "IStorageFileImpl.h"
#include "RefObjQIPtr.h"
#include "IStreamFileImpl.h"
#include "IEnumImpl.h"

namespace IFacesImpl
{

  IStorageFileImpl::IStorageFileImpl()
  {
  }

  IStorageFileImpl::~IStorageFileImpl()
  {
  }

  RetCode IStorageFileImpl::CreateStorage(const char *name, IFaces::IStorage **storage)
  {
    Common::ISyncObject Locker(GetSynObj());
    if (!name)
      return retBadParam;
    if (!StorageName.empty())
      return retFail;
    try
    {
      Common::RefObjQIPtr<IFaces::IStorage> NewStg(OpenFileStorage(name, true, GetSynObj()));
      if (!System::CreateDir(StorageName.c_str()))
        return retFail;
      if (NewStg.QueryInterface(storage) != retOk)
        System::RemoveDir(StorageName.c_str());
    }
    catch (std::exception &)
    {
    }
    return retFail;
  }

  RetCode IStorageFileImpl::OpenStorage(const char *name, IFaces::IStorage **storage)
  {
    Common::ISyncObject Locker(GetSynObj());
    if (!name)
      return retBadParam;
    if (!StorageName.empty())
      return retFail;
    try
    {
      return OpenFileStorage(name, false, GetSynObj()).QueryInterface(storage);
    }
    catch (std::exception &)
    {
    }
    return retFail;
  }

  RetCode IStorageFileImpl::CreateStream(const char *name, IFaces::IStream **stream)
  {
    Common::ISyncObject Locker(GetSynObj());
    try
    {
      std::string FileName = StorageName + "/";
      FileName += name;
      Common::RefObjQIPtr<IFaces::IStream> NewStream(OpenFileStream(FileName.c_str(), true, GetSynObj()));
      return NewStream.QueryInterface(stream);
    }
    catch (std::exception &)
    {
    }
    return retFail;
  }

  RetCode IStorageFileImpl::OpenStream(const char *name, IFaces::IStream **stream)
  {
    Common::ISyncObject Locker(GetSynObj());
    try
    {
      std::string FileName = StorageName + "/";
      FileName += name;
      Common::RefObjQIPtr<IFaces::IStream> NewStream(OpenFileStream(FileName.c_str(), false, GetSynObj()));
      return NewStream.QueryInterface(stream);
    }
    catch (std::exception &)
    {
    }
    return retFail;
  }

  RetCode IStorageFileImpl::RemoveItem(const char *name)
  {
    Common::ISyncObject Locker(GetSynObj());
    return System::RemoveDir(name) ? retOk : retFail;
  }

  RetCode IStorageFileImpl::Enum(IFaces::IEnum **items) const
  {
    Common::ISyncObject Locker(GetSynObj());
    System::DirItemPoolPtr DirItems = System::EnumDir(StorageName.c_str());
    if (!DirItems.get())
      return retFail;
    try
    {
      Common::RefObjPtr<IEnumImpl> NewEnum(CreateEnum(GetSynObj()));
      for (System::DirItemPool::const_iterator i = DirItems->begin() ; i != DirItems->end() ; ++i)
      {
        std::string NewItemName = StorageName + "/";
        NewItemName += i->first;
        Common::RefObjPtr<IFaces::IBase> NewItem;
        if (i->second)
          NewItem = Common::RefObjQIPtr<IFaces::IBase>(OpenFileStorage(NewItemName, false, GetSynObj()));
        else
          NewItem = Common::RefObjQIPtr<IFaces::IBase>(OpenFileStream(NewItemName, false, GetSynObj()));
        if (!NewItem.Get())
          return retFail;
        NewEnum->AddItem(NewItem);
      }
      return NewEnum.QueryInterface(items);
    }
    catch (std::exception &)
    {
    }
    return retFail;
  }

  void IStorageFileImpl::Init(const std::string &name, bool isNew)
  {
    if (name.empty())
      throw IStorageFileImplException("Empty dir name");
    if (isNew && !System::ExistsDir(name.c_str()))
      throw IStorageFileImplException("Dir not exists");
    StorageName = name;
  }

  Common::RefObjPtr<IStorageFileImpl>
    OpenFileStorage(const std::string &name, bool createNew,
                    const Common::ISynObj &syn)
  {
    Common::RefObjPtr<IStorageFileImpl> Ret =
      Common::IBaseImpl<IStorageFileImpl>::CreateWithSyn(syn);
    Ret->Init(name, createNew);
    return Ret;
  }

}
