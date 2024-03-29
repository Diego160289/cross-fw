//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __ISTREAMMEMORYIMPL_H__
#define __ISTREAMMEMORYIMPL_H__

#include "IFacesTools.h"
#include "Exceptions.h"
#include "IRawDataBufferImpl.h"
#include "RefObjQIPtr.h"


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
          Common::TypeListAdapter
            <
              IFaces::IStream,
              IRawDataBufferImpl
            >
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
    unsigned long Cursor;
  };

  Common::RefObjPtr<IFaces::IStream>
    OpenMemoryStream(const Common::ISynObj &syn);

  template <typename TSyn>
  Common::RefObjPtr<IFaces::IStream>
    OpenMemoryStream()
  {
    Common::RefObjPtr<IFaces::IStream> Ret;
    Common::IBaseImpl<IStreamMemoryImpl>::Create<TSyn>().QueryInterface(Ret.GetPPtr());
    return Ret;
  }

  Common::RefObjPtr<IFaces::IRawDataBuffer>
    StreamToBuf(Common::RefObjPtr<IFaces::IStream> stream, const Common::ISynObj &syn);

  template <typename TSyn>
  Common::RefObjPtr<IFaces::IRawDataBuffer>
    StreamToBuf(Common::RefObjPtr<IFaces::IStream> stream)
  {
    Common::RefObjPtr<IFaces::IStream> NewStream =
      Common::IBaseImpl<IStreamMemoryImpl>::Create<TSyn>();
    if (stream->CopyTo(NewStream.Get()) != retOk)
      return Common::RefObjPtr<IFaces::IRawDataBuffer>();
    return Common::RefObjQIPtr<IFaces::IRawDataBuffer>(NewStream);
  }

}

#endif // !__ISTREAMMEMORYIMPL_H__
