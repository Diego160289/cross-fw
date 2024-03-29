//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __NOCOPYABLE_H__
#define __NOCOPYABLE_H__


namespace Common
{
  class NoCopyable
  {
  protected:
    NoCopyable()
    {
    }
  private:
    NoCopyable(const NoCopyable&);
    NoCopyable& operator =(const NoCopyable&);
  };
}

#endif	// !__NOCOPYABLE_H__
