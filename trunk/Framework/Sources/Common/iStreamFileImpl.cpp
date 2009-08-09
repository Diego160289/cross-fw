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
    return retFail;
  }

  RetCode IStreamFileImpl::Read(void *buf, unsigned long bufSize, unsigned long *readBytes)
  {
    return retFail;
  }

  RetCode IStreamFileImpl::Write(const void *buf, unsigned long bytes)
  {
    return retFail;
  }

  RetCode IStreamFileImpl::SeekToBegin()
  {
    return retFail;
  }

  RetCode IStreamFileImpl::SeekToEnd()
  {
    return retFail;
  }

  RetCode IStreamFileImpl::SeekTo(unsigned long pos)
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
