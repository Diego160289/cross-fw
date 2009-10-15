#ifndef __ISTORAGEFILEIMPL_H__
#define __ISTORAGEFILEIMPL_H__

#include "IFacesTools.h"
#include "Exceptions.h"
#include "Dir.h"


namespace IFacesImpl
{

  using IFaces::RetCode;
  using IFaces::retOk;
  using IFaces::retFalse;
  using IFaces::retFail;
  using IFaces::retBadParam;

  DECLARE_RUNTIME_EXCEPTION(IStorageFileImpl)

  class IStorageFileImpl
    : public Common::CoClassBase
        <
          TYPE_LIST_1(IFaces::IStorage)
        >
  {
  public:
    DECLARE_UUID(d3c39113-a2e3-4d6e-ae48-4125bdf93fb1)

    IStorageFileImpl();
    virtual ~IStorageFileImpl();

    // IStorage
    virtual RetCode CreateStorage(const char *name, IFaces::IStorage **storage);
    virtual RetCode OpenStorage(const char *name, IFaces::IStorage **storage);
    virtual RetCode CreateStream(const char *name, IFaces::IStream **stream);
    virtual RetCode OpenStream(const char *name, IFaces::IStream **stream);
    virtual RetCode RemoveItem(const char *name);
    virtual RetCode Enum(IFaces::IEnum **items) const;

    void Init(const std::string &name, bool isNew);
  private:
    std::string StorageName;
  };

  Common::RefObjPtr<IStorageFileImpl>
    OpenFileStorage(const std::string &name, bool createNew,
                    const Common::ISynObj &syn);

  template <typename TSyn>
  Common::RefObjPtr<IStorageFileImpl>
    OpenFileStorage(const std::string &name, bool createNew)
  {
    Common::RefObjPtr<IStorageFileImpl> Ret =
      Common::IBaseImpl<IStorageFileImpl>::Create<TSyn>();
    Ret->Init(name, createNew);
    return Ret;
  }

}

#endif // !__ISTORAGEFILEIMPL_H__
