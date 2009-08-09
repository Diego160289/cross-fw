#include "FileImpl.h"

namespace System
{
  File::FileImpl::FileImpl(const char *fileName, File::FileMode mode)
    : FileHandle(INVALID_HANDLE_VALUE)
  {
    DWORD Access = 0;
    switch (mode)
    {
    case fmRead :
      Access = GENERIC_READ;
      break;
    case fmWrite :
      Access = GENERIC_WRITE;
      break;
    case fmReadWrite :
      Access = GENERIC_READ | GENERIC_WRITE;
    default :
      throw FileException("Unknown file mode");
    }
    DWORD ShareMode = 0;
    switch (mode)
    {
    case fmRead :
      ShareMode = FILE_SHARE_READ;
      break;
    case fmWrite :
      ShareMode = FILE_SHARE_WRITE;
      break;
    case fmReadWrite :
      ShareMode = FILE_SHARE_WRITE;
    default :
      throw FileException("Unknown file mode");
    }
    if ((FileHandle = CreateFileA(fileName, Access, ShareMode, NULL,
      mode == fmRead ? OPEN_EXISTING : CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
    {
      throw FileException("Can't open file");
    }
  }

  File::FileImpl::~FileImpl()
  {
    CloseHandle(FileHandle);
  }

  unsigned long File::FileImpl::GetSize() const
  {
    DWORD Size = 0;
    if (GetFileSize(FileHandle, &Size) == INVALID_FILE_SIZE)
      throw FileException("Can't get file size");
    return static_cast<unsigned long>(Size);
  }

  unsigned long File::FileImpl::Read(void *buf, unsigned long bufSize)
  {
    DWORD ReadBytes = static_cast<DWORD>(bufSize);
    if (!ReadFile(FileHandle, buf, static_cast<DWORD>(bufSize), &ReadBytes, NULL))
      throw FileException("Can't Read");
    return static_cast<unsigned long>(ReadBytes);
  }

  unsigned long File::FileImpl::Write(const void *buf, unsigned long bytes)
  {
    DWORD WriteBytes = static_cast<DWORD>(bytes);
    if (!WriteFile(FileHandle, buf, static_cast<DWORD>(bytes), &WriteBytes, NULL))
      throw FileException("Can't write");
    return static_cast<unsigned long>(WriteBytes);
  }

  void File::FileImpl::SeekToBegin()
  {
    if (SetFilePointer(FileHandle, 0, 0, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
      throw FileException("Can't seek to begin");
  }

  void File::FileImpl::SeekToEnd()
  {
    if (SetFilePointer(FileHandle, 0, 0, FILE_END) == INVALID_SET_FILE_POINTER)
      throw FileException("Can't seek to end");
  }

  void File::FileImpl::SeekTo(unsigned long pos)
  {
    if (!pos)
    {
      SeekToBegin();
      return;
    }
    if (SetFilePointer(FileHandle, static_cast<LONG>(pos), 0, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
      throw FileException("Can't seek to mew position");
  }

  void File::FileImpl::GetPos(unsigned long *pos) const
  {
    DWORD CurPos = SetFilePointer(FileHandle, 0, 0, FILE_CURRENT);
    if (CurPos == INVALID_SET_FILE_POINTER)
      throw FileException("Can't get current position");
    if (SetFilePointer(FileHandle, static_cast<LONG>(CurPos), 0, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
      throw FileException("Can't get current position");
    *pos = static_cast<unsigned long>(CurPos);
  }

  void File::FileImpl::Remove(const char *fileName)
  {
    if (!::DeleteFileA(fileName))
      throw FileException("Can't remove file");
  }

}
