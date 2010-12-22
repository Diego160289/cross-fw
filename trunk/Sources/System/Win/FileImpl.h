//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


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
    unsigned long GetSize() const;
    unsigned long Read(void *buf, unsigned long bufSize);
    unsigned long Write(const void *buf, unsigned long bytes);
    void SeekToBegin();
    void SeekToEnd();
    void SeekTo(unsigned long pos);
    unsigned long GetPos() const;
  private:
    HANDLE FileHandle;
  };
  
  void RemoveFileImpl(const char *fileName);
}

#endif  // !__FILEIMPL_H__
