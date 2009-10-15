#include "Dir.h"
#include "DirImpl.h"

namespace System
{

  bool ExistsDir(const char *dirName)
  {
    return ExistsDirImpl(dirName);
  }

  bool CreateDir(const char *dirName)
  {
    return CreateDirImpl(dirName);
  }

  bool RemoveDir(const char *dirName)
  {
    return RemoveDirImpl(dirName);
  }

  DirItemPoolPtr EnumDir(const char *dirName)
  {
    return EnumDirImpl(dirName);
  }

}
