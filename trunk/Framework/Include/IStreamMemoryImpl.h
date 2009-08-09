#ifndef __ISTREAMMEMORYIMPL_H__
#define __ISTREAMMEMORYIMPL_H__

#include "IFacesTools.h"
#include "Exceptions.h"


namespace IFacesImpl
{

  using IFaces::RetCode;
  using IFaces::retOk;
  using IFaces::retFalse;
  using IFaces::retFail;
  using IFaces::retBadParam;

  DECLARE_RUNTIME_EXCEPTION(IStreamMemoryImpl)

  class IStreamMemoryImpl
    : public Common::CoClassBase
        <
          TYPE_LIST_1(IFaces::IStream)
        >
  {
  public:
    DECLARE_UUID(6d8f1b4f-544a-4a3a-90c9-e88fca669de5)

    IStreamMemoryImpl();
    virtual ~IStreamMemoryImpl();

    // IStream
    virtual RetCode GetSize(unsigned long *size) const;
    virtual RetCode Read(void *buf, unsigned long bufSize, unsigned long *readBytes);
    virtual RetCode Write(const void *buf, unsigned long bytes);
    virtual RetCode SeekToBegin();
    virtual RetCode SeekToEnd();
    virtual RetCode SeekTo(unsigned long pos);
    virtual RetCode GetPos(unsigned long *pos) const;
    virtual RetCode CopyTo(IStream *dest) const;

  private:
  };

  Common::RefObjPtr<IStreamMemoryImpl>
    OpenMemoryStream(const Common::ISynObj &syn);

  template <typename TSyn>
  Common::RefObjPtr<IStreamMemoryImpl>
    OpenMemoryStream()
  {
    return Common::IBaseImpl<IStreamMemoryImpl>::Create<TSyn>();
  }

}

#endif // !__ISTREAMMEMORYIMPL_H__
