#ifndef __FILEIMPL_H__
#define __FILEIMPL_H__

#include "File.h"

#include <windows.h>


namespace System
{
  class File::FileImpl
    : Common::NoCopyable
  {
  public:
    FileImpl(const char *fileName, File::FileMode mode);
    ~FileImpl();
    static void Remove(const char *fileName);
  private:
    HANDLE FileHandle;
  };
}

#endif  // !__FILEIMPL_H__
