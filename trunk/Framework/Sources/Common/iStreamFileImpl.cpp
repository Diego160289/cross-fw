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
    Common::ISyncObject Locker(GetSynObj());
    return retFail;
  }

  RetCode IStreamFileImpl::Read(void *buf, unsigned long bufSize, unsigned long *readBytes)
  {
    Common::ISyncObject Locker(GetSynObj());
    return retFail;
  }

  RetCode IStreamFileImpl::Write(const void *buf, unsigned long bytes)
  {
    Common::ISyncObject Locker(GetSynObj());
    return retFail;
  }

  RetCode IStreamFileImpl::SeekToBegin()
  {
    Common::ISyncObject Locker(GetSynObj());
    return retFail;
  }

  RetCode IStreamFileImpl::SeekToEnd()
  {
    Common::ISyncObject Locker(GetSynObj());
    return retFail;
  }

  RetCode IStreamFileImpl::SeekTo(unsigned long pos)
  {
    Common::ISyncObject Locker(GetSynObj());
    return retFail;
  }

  RetCode IStreamFileImpl::GetPos(unsigned long *pos) const
  {
    return retFail;
  }

  RetCode IStreamFileImpl::CopyTo(IStream *dest) const
  {
    return retFail;
  }

  void IStreamFileImpl::Init(const std::string &name, bool isNew)
  {
    if (name.empty())
      throw IStreamFileImplException("Emptuy file name");
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
