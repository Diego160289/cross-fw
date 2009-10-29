#ifndef __ISBASEOF_H__
#define __ISBASEOF_H__

namespace Common
{

  template
  <
    typename TBase,
    typename TDerived
  >
  class IsBaseOf
  {
    IsBaseOf();
    ~IsBaseOf();
    struct Yes
    {
      char c[1];
    };
    struct No
    {
      char c[2];
    };
    static Yes Check(const TBase *);
    static No Check(const void *);
  public:
    enum { IsBase = sizeof(Check((TDerived*)0)) == sizeof(Yes) };
  };


}

#endif // !__ISBASEOF_H__
