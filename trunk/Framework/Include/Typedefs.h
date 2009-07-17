#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

#include <memory>
#include <vector>
#include <string>
#include <map>

namespace Common
{
  typedef std::vector<char> CharVector;
  typedef std::auto_ptr<CharVector> CharVectorPtr;
  typedef std::vector<std::string> StringVector;
  typedef std::auto_ptr<std::string> StringPtr;
  typedef std::map<std::string, std::string> StringMap;
  typedef std::auto_ptr<StringMap> StringMapPtr;
}

#endif  // !__TYPEDEFS_H__
