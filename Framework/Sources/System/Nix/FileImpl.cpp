#include "FileImpl.h"

#include <string>

namespace System
{
  File::FileImpl::FileImpl(const char *fileName, File::FileMode mode)
    : FileHandle(0)
  {
    std::string OpenMode;
    switch (mode)
    {
    case fmWrite :
    case fmReadWrite :
      OpenMode = "a+b";
      break;
    case fmRead :
      OpenMode = "rb";
      break;
    default :
      throw FileException("Unknown file mode");
    }
    if (!(FileHandle = fopen(fileName, OpenMode.c_str())))
      throw FileException("Can't open file");
  }

  File::FileImpl::~FileImpl()
  {
    fclose(FileHandle);
  }

  unsigned long File::FileImpl::GetSize() const
  {
    unsigned long CurPos = GetPos();
    const_cast<FileImpl*>(this)->SeekToEnd();
    try
    {
      unsigned long EndPos = GetPos();
      const_cast<FileImpl*>(this)->SeekTo(CurPos);
      return EndPos;
    }
    catch (...)
    {
      const_cast<FileImpl*>(this)->SeekTo(CurPos);
      throw;
    }
    return 0;
  }

  unsigned long File::FileImpl::Read(void *buf, unsigned long bufSize)
  {
    size_t ReadBytes = fread(buf, 1, bufSize, FileHandle);
    if (ReadBytes < 0)
      throw FileException("Can't read");
    return static_cast<unsigned long>(ReadBytes);
  }

  unsigned long File::FileImpl::Write(const void *buf, unsigned long bytes)
  {
    size_t WriteBytes = fwrite(buf, 1, bytes, FileHandle);
    if (WriteBytes < 0)
      throw FileException("Can't write");
    return static_cast<unsigned long>(WriteBytes);
  }

  void File::FileImpl::SeekToBegin()
  {
    if (fseek(FileHandle, 0, SEEK_SET))
      throw FileException("Can't seek to begin");
  }

  void File::FileImpl::SeekToEnd()
  {
    if (fseek(FileHandle, 0, SEEK_END))
      throw FileException("Can't seek to end");
  }

  void File::FileImpl::SeekTo(unsigned long pos)
  {
    if (!pos)
    {
      SeekToBegin();
      return;
    }
    if (fseek(FileHandle, static_cast<long>(pos), SEEK_SET))
      throw FileException("Can't seek to new position");
  }

  unsigned long File::FileImpl::GetPos() const
  {
    long Pos = ftell(FileHandle);
    if (Pos < 0)
      throw FileException("Can't get current position");
    return static_cast<unsigned long>(Pos);
  }

  void File::FileImpl::Remove(const char *fileName)
  {
    if (unlink(fileName))
      throw FileException("Can't remove file");
  }

}
