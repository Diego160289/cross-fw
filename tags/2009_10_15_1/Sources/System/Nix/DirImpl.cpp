#include "DirImpl.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include <sstream>


namespace System
{

  bool ExistsDirImpl(const char *dirName)
  {
    DIR *Dir = opendir(dirName);
    bool Ret = !!Dir;
    closedir(Dir);
    return Ret;
  }

  bool CreateDirImpl(const char *dirName)
  {
    struct
    {
      bool operator () (const char *name) const
      {
        return ExistsDirImpl(name) || mkdir(name, S_IRWXU) != -1;
      }
    } MakeDir;
    std::vector<std::string> Dirs;
    std::stringstream Path;
    Path << dirName;
    for (std::string s ; std::getline(Path, s, '/') ; Dirs.push_back(s));
    std::string CurPath;
    for (std::vector<std::string>::const_iterator i = Dirs.begin() ; i != Dirs.end() ; ++i)
    {
      if ((*i) == ".")
        continue;
      CurPath += *i;
      if (!MakeDir(CurPath.c_str()))
        return false;
      CurPath += "/";
    }
    return true;
  }

  bool RemoveDirImpl(const char *dirName)
  {
    std::vector<std::string> Dirs;
    std::stringstream Path;
    Path << dirName;
    std::string StartDir;
    std::getline(Path, StartDir, '/');
    struct
    {
      bool RemoveDir(const char *name) const
      {
        return ExistsDirImpl(name) && rmdir(name) != -1;
      }
      bool operator () ( std::string path) const
      {
        DirItemPoolPtr Items = EnumDirImpl(path.c_str());
	if (!Items.get())
	  return false;
	for (DirItemPool::const_iterator i = Items->begin() ; i != Items->end() ; ++i)
	{
	  std::string NewPath = path;
	  NewPath += "/";
	  NewPath += i->first;
	  if (i->second)
	    (*this)(NewPath);
	  else
	  {
	    if (unlink(NewPath.c_str()))
	      return false;
	  }
	}
	return RemoveDir(path.c_str());
      }
    } RemoveRecurcive;
    return RemoveRecurcive(StartDir);
  }

  DirItemPoolPtr EnumDirImpl(const char *dirName)
  {
    DIR *Dir = opendir(dirName);
    DirItemPoolPtr Ret;
    if (!Dir)
      return Ret;
    DirItemPoolPtr DirItems(new DirItemPool);
    for (dirent *DirData = readdir(Dir) ; DirData ; DirData = readdir(Dir))
    {
      std::string CurDirName(DirData->d_name);
      if (CurDirName == "." || CurDirName == "..")
        continue;
      struct stat ItemInfo = { 0 };
      std::string FullItemName = dirName;
      FullItemName += "/";
      FullItemName += DirData->d_name;
      if (stat(FullItemName.c_str(), &ItemInfo) == -1)
      {
        DirItems.reset(0);
        break;
      }
      DirItems->push_back(std::make_pair(CurDirName, (ItemInfo.st_mode & S_IFMT) == S_IFDIR));
    }
    if (DirItems.get())
      Ret = DirItems;
    closedir(Dir);
    return Ret;
  }

}
