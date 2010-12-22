//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "DirImpl.h"
#include "DllHolder.h"

#include <shlwapi.h>
#include <shellapi.h>


namespace System
{

  bool ExistsDirImpl(const char *dirName)
  {
    if (!dirName)
      return true;
    try
    {
      return !!DllHolder("shlwapi.dll").GetProc<BOOL (STDAPICALLTYPE *)(LPCSTR)>("PathFileExistsA")(dirName);
    }
    catch (std::exception &)
    {
      return false;
    }
    return true;
  }

  bool CreateDirImpl(const char *dirName)
  {
    return dirName &&
      (CreateDirectoryA(dirName, 0) || GetLastError() == ERROR_ALREADY_EXISTS);
  }

  bool RemoveDirImpl(const char *dirName)
  {
    if (!dirName)
      return true;
    SHFILEOPSTRUCTA SH = { 0 };
    SH.wFunc = FO_DELETE;
    SH.pFrom = dirName;
    SH.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
    try
    {
      return !DllHolder("shell32.dll").GetProc<int (STDAPICALLTYPE *)(LPSHFILEOPSTRUCTA)>("SHFileOperationA")(&SH);
    }
    catch (std::exception &)
    {
      return false;
    }
    return true;
  }

  DirItemPoolPtr EnumDirImpl(const char *dirName)
  {
    if (!dirName)
      return DirItemPoolPtr(0);
    WIN32_FIND_DATAA Ffd = { 0 };
    std::string Dir = dirName;
    Dir += "/*";
    HANDLE Find = FindFirstFileA(Dir.c_str(), &Ffd);
    if (Find == INVALID_HANDLE_VALUE) 
      return DirItemPoolPtr(0);
    DirItemPoolPtr Ret(new DirItemPool);
    do
    {
      std::string ItemName = Ffd.cFileName;
      if (Ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
        if (ItemName.empty() || ItemName == "." || ItemName == "..")
          continue;
        Ret->push_back(std::make_pair(ItemName, true));
        continue;
      }
      if (ItemName.empty())
        continue;
      Ret->push_back(std::make_pair(ItemName, false));
    }
    while (FindNextFileA(Find, &Ffd) != 0);
    if (GetLastError() != ERROR_NO_MORE_FILES) 
    {
      FindClose(Find);
      return DirItemPoolPtr(0);
    }
    FindClose(Find);
    return Ret;
  }

}
