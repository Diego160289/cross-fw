#ifndef __FILEIMPL_H__
#define __FILEIMPL_H__

#include "File.h"
#include <stdio.h>


namespace System
{
  class File::FileImpl
    : Common::NoCopyable
  {
  public:
    FileImpl(const char *fileName, File::FileMode mode);
    ~FileImpl();
    unsigned long GetSize() const;
    unsigned long Read(void *buf, unsigned long bufSize);
    unsigned long Write(const void *buf, unsigned long bytes);
    void SeekToBegin();
    void SeekToEnd();
    void SeekTo(unsigned long pos);
    unsigned long GetPos() const;
    static void Remove(const char *fileName);
  private:
    FILE *FileHandle;
  };
}

#endif  // !__FILEIMPL_H__
