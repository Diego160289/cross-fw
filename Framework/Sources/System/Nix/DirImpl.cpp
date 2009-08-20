#include "DirImpl.h"
#include "DllHolder.h"

namespace System
{

  bool ExistsDirImpl(const char *dirName)
  {
    return false;
  }

  bool CreateDirImpl(const char *dirName)
  {
    return false;
  }

  bool RemoveDirImpl(const char *dirName)
  {
    return false;
  }

  DirItemPoolPtr EnumDirImpl(const char *dirName)
  {
    return DirItemPoolPtr(0);
  }

}
