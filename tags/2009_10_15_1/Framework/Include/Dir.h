#ifndef __DIR_H__
#define __DIR_H__

#include <vector>
#include <string>
#include <memory>

namespace System
{
  bool ExistsDir(const char *dirName);
  bool CreateDir(const char *dirName);
  bool RemoveDir(const char *dirName);

  typedef std::pair<std::string/*dir item name*/, bool/*is dir*/> DirItem;
  typedef std::vector<DirItem> DirItemPool;
  typedef std::auto_ptr<DirItemPool> DirItemPoolPtr;

  DirItemPoolPtr EnumDir(const char *dirName);
}

#endif // !__DIR_H__
