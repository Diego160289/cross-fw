#ifndef __COMMONUTILS_H__
#define __COMMONUTILS_H__

#include "Typedefs.h"

namespace Common
{
  std::string GetChecksumMD5(const void* data, unsigned length);

  std::string BinToBase64(const void *data, unsigned bytes);
  CharVectorPtr Base64ToBin(const char *base64Str);

  const std::string GetValueSromStringMap(const std::string &key, const StringMap &m);
}

#endif  // !__COMMONUTILS_H__
