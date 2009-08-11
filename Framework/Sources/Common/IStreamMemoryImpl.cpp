#include "IStreamMemoryImpl.h"


namespace IFacesImpl
{

  IStreamMemoryImpl::IStreamMemoryImpl()
    : Cursor(0)
  {
  }

  IStreamMemoryImpl::~IStreamMemoryImpl()
  {
  }

  RetCode IStreamMemoryImpl::GetSize(unsigned long *size) const
  {
    if (!size)
      return retBadParam;
    Common::ISyncObject Locker(GetSynObj());
    *size = IRawDataBufferImpl::GetSize();
    return retOk;
  }

  RetCode IStreamMemoryImpl::Read(void *buf, unsigned long bufSize, unsigned long *readBytes)
  {
    if (!buf || !readBytes)
      return retBadParam;
    Common::ISyncObject Locker(GetSynObj());
    unsigned long Size = IRawDataBufferImpl::GetSize();
    if (!Size)
    {
      *readBytes = 0;
      return retOk;
    }
    unsigned long Dif = Size - Cursor;
    *readBytes = Dif < bufSize ? Dif : bufSize;
    const char *Data = reinterpret_cast<const char *>(GetData());
    char *OutputBuf = reinterpret_cast<char *>(buf);
    for (unsigned i = 0 ; i < *readBytes ; OutputBuf[i++] = Data[Cursor++]);
    return retOk;
  }

  RetCode IStreamMemoryImpl::Write(const void *buf, unsigned long bytes)
  {
    if (!buf)
      return retBadParam;
    Common::ISyncObject Locker(GetSynObj());
    unsigned long Size = IRawDataBufferImpl::GetSize();
    char *Data = reinterpret_cast<char *>(GetData());
    if (!Data || bytes >= Size - Cursor)
    {
      Resize(bytes + Cursor);
      Data = reinterpret_cast<char *>(GetData());
    }
    const char *InputBuf = reinterpret_cast<const char *>(buf);
    for (unsigned i = 0 ; i < bytes ; Data[Cursor++] = InputBuf[i++]);
    return retOk;
  }

  RetCode IStreamMemoryImpl::SeekToBegin()
  {
    Common::ISyncObject Locker(GetSynObj());
    Cursor = 0;
    return retOk;
  }

  RetCode IStreamMemoryImpl::SeekToEnd()
  {
    Common::ISyncObject Locker(GetSynObj());
    unsigned long Size = IRawDataBufferImpl::GetSize();
    if (!Size)
      Cursor = 0;
    else
      Cursor = Size;
    return retOk;
  }

  RetCode IStreamMemoryImpl::SeekTo(unsigned long pos)
  {
    Common::ISyncObject Locker(GetSynObj());
    if (pos >= IRawDataBufferImpl::GetSize())
      return retBadParam;
    Cursor = pos;
    return retOk;
  }

  RetCode IStreamMemoryImpl::GetPos(unsigned long *pos) const
  {
    if (!pos)
      return retFail;
    Common::ISyncObject Locker(GetSynObj());
    *pos = Cursor;
    return retOk;
  }

  RetCode IStreamMemoryImpl::CopyTo(IStream *dest) const
  {
    Common::ISyncObject Locker(GetSynObj());
    Common::RefObjPtr<IFaces::IStream> Dest(dest);
    if (!Dest.Get())
      return retBadParam;
    return Dest->Write(GetData(), IRawDataBufferImpl::GetSize());
  }


  Common::RefObjPtr<IFaces::IStream>
    OpenMemoryStream(const Common::ISynObj &syn)
  {
    Common::RefObjPtr<IFaces::IStream> Ret;
    Common::IBaseImpl<IStreamMemoryImpl>::CreateWithSyn(syn).QueryInterface(Ret.GetPPtr());
    return Ret;
  }

  Common::RefObjPtr<IFaces::IRawDataBuffer>
    StreamToBuf(Common::RefObjPtr<IFaces::IStream> stream, const Common::ISynObj &syn)
  {
    Common::RefObjPtr<IFaces::IStream> NewStream =
      Common::IBaseImpl<IStreamMemoryImpl>::CreateWithSyn(syn);
    if (stream->CopyTo(NewStream.Get()) != retOk)
      return Common::RefObjPtr<IFaces::IRawDataBuffer>();
    return Common::RefObjQIPtr<IFaces::IRawDataBuffer>(NewStream);
  }

}
