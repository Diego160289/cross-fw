//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __FILE_H__
#define __FILE_H__

#include "NoCopyable.h"
#include "Exceptions.h"


namespace System
{
  DECLARE_RUNTIME_EXCEPTION(File)

  class File
    : private Common::NoCopyable
  {
  public:
    enum FileMode { fmRead, fmWrite, fmReadWrite };
    File(const char *fileName, FileMode mode);
    virtual ~File();
    unsigned long GetSize() const;
    unsigned long Read(void *buf, unsigned long bufSize);
    unsigned long Write(const void *buf, unsigned long bytes);
    void SeekToBegin();
    void SeekToEnd();
    void SeekTo(unsigned long pos);
    unsigned long GetPos() const;
  private:
    class FileImpl;
    FileImpl *Impl;
  };
  
  void RemoveFile(const char *fileName);
}

#endif  // !__FILE_H__
