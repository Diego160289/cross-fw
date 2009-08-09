#include "IStreamMemoryImpl.h"


namespace IFacesImpl
{

  IStreamMemoryImpl::IStreamMemoryImpl()
  {
  }

  IStreamMemoryImpl::~IStreamMemoryImpl()
  {
  }

  RetCode IStreamMemoryImpl::GetSize(unsigned long *size) const
  {
    Common::ISyncObject Locker(GetSynObj());
    return retFail;
  }

  RetCode IStreamMemoryImpl::Read(void *buf, unsigned long bufSize, unsigned long *readBytes)
  {
    Common::ISyncObject Locker(GetSynObj());
    return retFail;
  }

  RetCode IStreamMemoryImpl::Write(const void *buf, unsigned long bytes)
  {
    Common::ISyncObject Locker(GetSynObj());
    return retFail;
  }

  RetCode IStreamMemoryImpl::SeekToBegin()
  {
    Common::ISyncObject Locker(GetSynObj());
    return retFail;
  }

  RetCode IStreamMemoryImpl::SeekToEnd()
  {
    Common::ISyncObject Locker(GetSynObj());
    return retFail;
  }

  RetCode IStreamMemoryImpl::SeekTo(unsigned long pos)
  {
    Common::ISyncObject Locker(GetSynObj());
    return retFail;
  }

  RetCode IStreamMemoryImpl::GetPos(unsigned long *pos) const
  {
    Common::ISyncObject Locker(GetSynObj());
    return retFail;
  }

  RetCode IStreamMemoryImpl::CopyTo(IStream *dest) const
  {
    Common::ISyncObject Locker(GetSynObj());
    return retFail;
  }


  Common::RefObjPtr<IStreamMemoryImpl>
    OpenMemoryStream(const Common::ISynObj &syn)
  {
    return Common::IBaseImpl<IStreamMemoryImpl>::CreateWithSyn(syn);
  }

}
