#ifndef __FLASHCTRLHOLDER_H__
#define __FLASHCTRLHOLDER_H__

#include "../../../../Framework/Include/NoCopyable.h"
#include "../../../../Framework/Include/DllHolder.h"
#include "../../../../Framework/Include/Pointers.h"
#include "../../../../Framework/Include/Exceptions.h"

#include "WFExtensions.h"

#include <windows.h>

class FlashHandle
  : private Common::NoCopyable
{
public:
  typedef Common::SharedPtr<System::DllHolder> DllHolderPtr;
  FlashHandle(DllHolderPtr finbox);
  ~FlashHandle();
private:
  HANDLE File;
  HANDLE FileMapping;
  LPVOID FlashCtrlData;
  DllHolderPtr FInBox;
  typedef struct HFPC_
  {
    void* p;
  } *HFPC;
  HFPC Flash;
};


DECLARE_RUNTIME_EXCEPTION(FlashCtrlHolder)

class FlashCtrlHolder
  : private Common::NoCopyable
{
public:
  FlashCtrlHolder();
  ~FlashCtrlHolder();
private:
  Common::SharedPtr<FlashHandle> Flash;
};

#endif // !__FLASHCTRLHOLDER_H__
