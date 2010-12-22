//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


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
