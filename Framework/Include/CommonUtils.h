#ifndef __COMMONUTILS_H__
#define __COMMONUTILS_H__

#include <string>

namespace Common
{
  std::string GetChecksumMD5(const void* data, unsigned length);
}

#endif  // !__COMMONUTILS_H__
