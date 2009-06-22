#include "FileImpl.h"

namespace System
{
  File::FileImpl::FileImpl(const char *fileName, File::FileMode mode)
  {
  }

  File::FileImpl::~FileImpl()
  {
  }

  void File::FileImpl::Remove(const char *fileName)
  {
    if (!::DeleteFileA(fileName))
      throw FileException(std::string("Can't remove \"") + fileName + std::string("\""));
  }

}
