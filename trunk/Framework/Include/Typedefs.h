#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

#include <memory>
#include <vector>
#include <string>

namespace Common
{
  typedef std::vector<char> CharVector;
  typedef std::auto_ptr<CharVector> CharVectorPtr;
}

#endif  // !__TYPEDEFS_H__
