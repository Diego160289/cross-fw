#include "IStreamHelper.h"


namespace IFacesImpl
{

  IStreamHelper::IStreamHelper(IStreamPtr stream)
    : Stream(stream)
  {
    if (!Stream.Get())
      throw IStreamHelperException("Empty stream pointer");
  }

  unsigned long IStreamHelper::GetSize() const
  {
    unsigned long Size = 0;
    if (Stream->GetSize(&Size) != retOk)
      throw IStreamHelperException("Can't get stream size");
    return Size;
  }

  unsigned long IStreamHelper::Read(void *buf, unsigned long bufSize)
  {
    unsigned long ReadBytes = 0;
    if (Stream->Read(buf, bufSize, &ReadBytes) != retOk)
      throw IStreamHelperException("Can't read from stream");
    return ReadBytes;
  }

  void IStreamHelper::Write(const void *buf, unsigned long bytes)
  {
    if (Stream->Write(buf, bytes) != retOk)
      throw IStreamHelperException("Can't write to stream");
  }

  void IStreamHelper::SeekToBegin()
  {
    if (Stream->SeekToBegin() != retOk)
      throw IStreamHelperException("Can't seek to begin stream");
  }

  void IStreamHelper::SeekToEnd()
  {
    if (Stream->SeekToEnd() != retOk)
      throw IStreamHelperException("Can't seek to end stream");
  }

  void IStreamHelper::SeekTo(unsigned long pos)
  {
    if (Stream->SeekTo(pos) != retOk)
      throw IStreamHelperException("Can't seek");
  }

  unsigned long IStreamHelper::GetPos() const
  {
    unsigned long Pos = 0;
    if (Stream->GetPos(&Pos) != retOk)
      throw IStreamHelperException("Can't get stream position");
    return Pos;
  }

  void IStreamHelper::CopyTo(IStreamPtr dest) const
  {
    if (Stream->CopyTo(dest.Get()) != retOk)
      throw IStreamHelperException("Can't copy stream");
  }

}
