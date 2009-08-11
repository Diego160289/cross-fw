#ifndef __COMMONUTILS_H__
#define __COMMONUTILS_H__

#include "Typedefs.h"

#include <sstream>

namespace Common
{
  std::string GetChecksumMD5(const void* data, unsigned length);

  std::string BinToBase64(const void *data, unsigned bytes);
  CharVectorPtr Base64ToBin(const char *base64Str);

  const std::string GetValueSromStringMap(const std::string &key, const StringMap &m);

  template <typename T>
  void Replace(std::basic_string<T> *str,
               const std::basic_string<T> &str1,
               const std::basic_string<T> &str2)
  {
    std::basic_string<T>::size_type StrLen =
      str1.length() > str2.length() ? str1.length() : str2.length();
    for (std::basic_string<T>::size_type i = str->find(str1) ;
      i != std::basic_string<T>::npos ;
      i = str->find(str1, i + StrLen))
    {
      str->replace(i, str1.length(), str2.c_str(), str2.length());
    }
  }

  template <typename T>
  std::auto_ptr<std::vector<std::basic_string<T> > >
    SplitString(const std::basic_string<T> &str, T splitter)
  {
    std::auto_ptr<std::vector<std::basic_string<T> > > Ret(new std::vector<std::basic_string<T> >);
    std::basic_stringstream<T, std::char_traits<T>, std::allocator<T> > Io;
    Io << str;
    for (std::basic_string<T> s ; std::getline(Io, s, splitter) ; Ret->push_back(s));
    return Ret;
  }
}

#endif  // !__COMMONUTILS_H__
