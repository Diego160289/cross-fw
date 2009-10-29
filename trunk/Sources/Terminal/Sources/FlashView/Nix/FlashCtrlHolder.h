#ifndef __FLASHCTRLHOLDER_H__
#define __FLASHCTRLHOLDER_H__

#include "../../../../Framework/Include/NoCopyable.h"
#include "../../../../Framework/Include/Exceptions.h"

#include "Terminal.h"
#include "WindowMessage.h"


DECLARE_RUNTIME_EXCEPTION(FlashCtrlHolder)

class FlashCtrlHolder
  : private Common::NoCopyable
{
public:
  FlashCtrlHolder();
  ~FlashCtrlHolder();
  void Create();
  void Destroy();
  long OnMessage(const IFaces::WindowMessage &msg);
private:
};

#endif // !__FLASHCTRLHOLDER_H__
