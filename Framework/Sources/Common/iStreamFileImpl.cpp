#include "IStreamFileImpl.h"


namespace IFacesImpl
{

  IStreamFileImpl::IStreamFileImpl()
  {
  }

  IStreamFileImpl::~IStreamFileImpl()
  {
  }

  RetCode IStreamFileImpl::GetSize(unsigned long *size) const
  {
    if (!size)
      return retBadParam;
    Common::ISyncObject Locker(GetSynObj());
    if (!File.Get())
      return retFail;
    try
    {
      *size = File->GetSize();
    }
    catch (std::exception &)
    {
      return retFail;
    }
    return retOk;
  }

  RetCode IStreamFileImpl::Read(void *buf, unsigned long bufSize, unsigned long *readBytes)
  {
    if (!readBytes)
      return retBadParam;
    Common::ISyncObject Locker(GetSynObj());
    if (!File.Get())
      return retFail;
    try
    {
      *readBytes = File->Read(buf, bufSize);
    }
    catch (std::exception &)
    {
      return retFail;
    }
    return retOk;
  }

  RetCode IStreamFileImpl::Write(const void *buf, unsigned long bytes)
  {
    Common::ISyncObject Locker(GetSynObj());
    if (!File.Get())
      return retFail;
    try
    {
      const char *Buf = reinterpret_cast<const char *>(buf);
      for (unsigned long i = bytes ; i ; )
      {
        unsigned long WriteBytes = File->Write(&Buf[bytes - i], i);
        if (!WriteBytes)
          return retFalse;
        i -= WriteBytes;
      }
    }
    catch (std::exception &)
    {
      return retFail;
    }
    return retOk;
  }

  RetCode IStreamFileImpl::SeekToBegin()
  {
    Common::ISyncObject Locker(GetSynObj());
    if (!File.Get())
      return retFail;
    try
    {
      File->SeekToBegin();
    }
    catch (std::exception &)
    {
      return retFail;
    }
    return retOk;
  }

  RetCode IStreamFileImpl::SeekToEnd()
  {
    Common::ISyncObject Locker(GetSynObj());
    if (!File.Get())
      return retFail;
    try
    {
      File->SeekToEnd();
    }
    catch (std::exception &)
    {
      return retFail;
    }
    return retOk;
  }

  RetCode IStreamFileImpl::SeekTo(unsigned long pos)
  {
    Common::ISyncObject Locker(GetSynObj());
    if (!File.Get())
      return retFail;
    try
    {
      File->SeekTo(pos);
    }
    catch (std::exception &)
    {
      return retFail;
    }
    return retOk;
  }

  RetCode IStreamFileImpl::GetPos(unsigned long *pos) const
  {
    if (!pos)
      return retBadParam;
    Common::ISyncObject Locker(GetSynObj());
    if (!File.Get())
      return retFail;
    try
    {
      *pos = File->GetPos();
    }
    catch (std::exception &)
    {
      return retFail;
    }
    return retOk;
  }

  RetCode IStreamFileImpl::CopyTo(IStream *dest) const
  {
    Common::ISyncObject Locker(GetSynObj());
    if (!File.Get())
      return retFail;
    Common::RefObjPtr<IFaces::IStream> Dest(dest);
    if (!Dest.Get())
      return retFail;
    try
    {
      unsigned long FileSize = File->GetSize();
      if (!FileSize)
        return retOk;
      unsigned long CurPos = File->GetPos();
      File->SeekToBegin();
      const unsigned long MaxBufSize = 32 * 1024 * 1024;
      unsigned long BufSize = MaxBufSize > FileSize ? FileSize : MaxBufSize;
      std::vector<char> Buf(BufSize, 0);
      for (unsigned long i = File->Read(&Buf[0], BufSize) ; i ; i = File->Read(&Buf[0], BufSize))
      {
        if (Dest->Write(&Buf[0], i) != retOk)
          return retFail;
      }
      File->SeekTo(CurPos);
    }
    catch (std::exception &)
    {
      return retFail;
    }
    return retOk;
  }

  void IStreamFileImpl::Init(const std::string &name, bool isNew)
  {
    if (name.empty())
      throw IStreamFileImplException("Emptuy file name");
    if (!isNew)
      System::File(name.c_str(), System::File::fmRead);
    File = new System::File(name.c_str(), System::File::fmReadWrite);
  }

  Common::RefObjPtr<IStreamFileImpl>
    OpenFileStream(const std::string &name, bool createNew, const Common::ISynObj &syn)
  {
    Common::RefObjPtr<IStreamFileImpl> Ret =
      Common::IBaseImpl<IStreamFileImpl>::CreateWithSyn(syn);
    Ret->Init(name, createNew);
    return Ret;
  }

}
