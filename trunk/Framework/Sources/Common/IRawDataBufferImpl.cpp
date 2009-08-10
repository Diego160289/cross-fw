#include "IRawDataBufferImpl.h"

#include <iterator>

namespace IFacesImpl
{

  IRawDataBufferImpl::IRawDataBufferImpl()
  {
  }
  
  IRawDataBufferImpl::~IRawDataBufferImpl()
  {
  }

  void* IRawDataBufferImpl::GetData()
  {
    Common::ISyncObject Locker(GetSynObj());
    return Buf.empty() ? 0 : &Buf[0];
  }

  const void* IRawDataBufferImpl::GetData() const
  {
    Common::ISyncObject Locker(GetSynObj());
    return Buf.empty() ? 0 : &Buf[0];
  }

  unsigned long IRawDataBufferImpl::GetSize() const
  {
    Common::ISyncObject Locker(GetSynObj());
    return static_cast<unsigned long>(Buf.size());
  }

  void IRawDataBufferImpl::SetData(const void *data, unsigned long bytes)
  {
    Common::ISyncObject Locker(GetSynObj());
    Buf.swap(Common::CharVector());
    Buf.reserve(bytes);
    std::copy(
      &reinterpret_cast<const char*>(data)[0],
      &reinterpret_cast<const char*>(data)[bytes],
      std::back_inserter(Buf)
      );
  }

  void IRawDataBufferImpl::AppendData(const void *data, unsigned long bytes)
  {
    Common::ISyncObject Locker(GetSynObj());
    std::copy(
      &reinterpret_cast<const char*>(data)[0],
      &reinterpret_cast<const char*>(data)[bytes],
      std::back_inserter(Buf)
      );
  }

  void IRawDataBufferImpl::Resize(unsigned long newSize)
  {
    Common::ISyncObject Locker(GetSynObj());
    Buf.resize(static_cast<Common::CharVector::size_type>(newSize));
  }

}
