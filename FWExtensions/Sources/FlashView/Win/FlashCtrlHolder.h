#ifndef __FLASHCTRLHOLDER_H__
#define __FLASHCTRLHOLDER_H__

#include "../../../../Framework/Include/NoCopyable.h"
#include "../../../../Framework/Include/DllHolder.h"
#include "../../../../Framework/Include/Pointers.h"

class FlashCtrlHolder
  : private Common::NoCopyable
{
public:
  FlashCtrlHolder();
  ~FlashCtrlHolder();
private:
  Common::SharedPtr<System::DllHolder> FInBox;
};

#endif // !__FLASHCTRLHOLDER_H__
