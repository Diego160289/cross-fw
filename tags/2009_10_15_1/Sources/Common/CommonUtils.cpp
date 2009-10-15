#include "CommonUtils.h"

#include <stdexcept>

namespace Common
{

  const std::string GetValueSromStringMap(const std::string &key, const StringMap &m)
  {
    StringMap::const_iterator Iter = m.find(key);
    if (Iter == m.end())
      throw std::logic_error("Value not found");
    return Iter->second;
  }

}
