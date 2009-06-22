#include "File.h"
#include "FileImpl.h"

namespace System
{
  File::File(const char *fileName, FileMode mode)
    : Impl(new FileImpl(fileName, mode))
  {
  }

  File::~File()
  {
  }

  void File::Remove(const char *fileName)
  {
    FileImpl::Remove(fileName);
  }
}
