//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


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
    delete Impl;
  }

  unsigned long File::GetSize() const
  {
    return Impl->GetSize();
  }

  unsigned long File::Read(void *buf, unsigned long bufSize)
  {
    return Impl->Read(buf, bufSize);
  }

  unsigned long File::Write(const void *buf, unsigned long bytes)
  {
    return Impl->Write(buf, bytes);
  }

  void File::SeekToBegin()
  {
    Impl->SeekToBegin();
  }

  void File::SeekToEnd()
  {
    Impl->SeekToEnd();
  }

  void File::SeekTo(unsigned long pos)
  {
    Impl->SeekTo(pos);
  }

  unsigned long File::GetPos() const
  {
    return Impl->GetPos();
  }

  
  void RemoveFile(const char *fileName)
  {
    RemoveFileImpl(fileName);
  }
}
