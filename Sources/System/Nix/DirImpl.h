#ifndef __DIRIMPL_H__
#define __DIRIMPL_H__

#include "Dir.h"


namespace System
{
  bool ExistsDirImpl(const char *dirName);
  bool CreateDirImpl(const char *dirName);
  bool RemoveDirImpl(const char *dirName);
  DirItemPoolPtr EnumDirImpl(const char *dirName);
}

#endif // !__DIRIMPL_H__
