#include "FileImpl.h"

namespace System
{
  File::FileImpl::FileImpl(const char *fileName, File::FileMode mode)
  {
  }

  File::FileImpl::~FileImpl()
  {
  }

  unsigned long File::FileImpl::GetSize() const
  {
    return 0;
  }

  unsigned long File::FileImpl::Read(void *buf, unsigned long bufSize)
  {
    return 0;
  }

  unsigned long File::FileImpl::Write(const void *buf, unsigned long bytes)
  {
    return 0;
  }

  void File::FileImpl::SeekToBegin()
  {
  }

  void File::FileImpl::SeekToEnd()
  {
  }

  void File::FileImpl::SeekTo(unsigned long pos)
  {
  }

  unsigned long File::FileImpl::GetPos() const
  {
    return 0;
  }

  void File::FileImpl::Remove(const char *fileName)
  {
    if (unlink(fileName))
      throw FileException("Can't remove file");
  }

}
