#ifndef __UITYPES_H__
#define __UITYPES_H__

namespace OGLUI
{

  class Rect
  {
  public:
    Rect(int left, int top, int right, int bottom)
      : Left(left)
      , Top(top)
      , Right(right)
      , Bottom(bottom)
    {
    }
    int GetLeft() const
    {
      return Left;
    }
    int GetTop() const
    {
      return Top;
    }
    int GetRight() const
    {
      return Right;
    }
    int GetBottom() const
    {
      return Bottom;
    }
  private:
    int Left;
    int Top;
    int Right;
    int Bottom;
  };

}

#endif  // !__UITYPES_H__
