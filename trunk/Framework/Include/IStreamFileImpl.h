#ifndef __ISTREAMFILEIMPL_H__
#define __ISTREAMFILEIMPL_H__

#include "IFacesTools.h"
#include "Exceptions.h"
#include "File.h"
#include "Pointers.h"


namespace IFacesImpl
{

  using IFaces::RetCode;
  using IFaces::retOk;
  using IFaces::retFalse;
  using IFaces::retFail;
  using IFaces::retBadParam;

  DECLARE_RUNTIME_EXCEPTION(IStreamFileImpl)

  class IStreamFileImpl
    : public Common::CoClassBase
        <
          TYPE_LIST_1(IFaces::IStream)
        >
  {
  public:
    DECLARE_UUID(26e45dfb-cd96-4d83-8302-6201e3e82160)

    IStreamFileImpl();
    virtual ~IStreamFileImpl();

    // IStream
    virtual RetCode GetSize(unsigned long *size) const;
    virtual RetCode Read(void *buf, unsigned long bufSize, unsigned long *readBytes);
    virtual RetCode Write(const void *buf, unsigned long bytes);
    virtual RetCode SeekToBegin();
    virtual RetCode SeekToEnd();
    virtual RetCode SeekTo(unsigned long pos);
    virtual RetCode GetPos(unsigned long *pos) const;
    virtual RetCode CopyTo(IStream *dest) const;

    void Init(const std::string &name, bool isNew);
  private:
    Common::SharedPtr<System::File> File;
  };

  Common::RefObjPtr<IStreamFileImpl>
    OpenFileStream(const std::string &name, bool createNew,
                    const Common::ISynObj &syn);

  template <typename TSyn>
  Common::RefObjPtr<IStreamFileImpl>
    OpenFileStream(const std::string &name, bool createNew)
  {
    Common::RefObjPtr<IStreamFileImpl> Ret =
      Common::IBaseImpl<IStreamFileImpl>::Create<TSyn>();
    Ret->Init(name, createNew);
    return Ret;
  }

}

#endif // !__ISTREAMFILEIMPL_H__
