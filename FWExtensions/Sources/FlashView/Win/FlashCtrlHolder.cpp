#include "FlashCtrlHolder.h"

FlashCtrlHolder::FlashCtrlHolder()
  : FInBox(new System::DllHolder("./f_in_box.dll")) // TODO: в пропертя
{
}

FlashCtrlHolder::~FlashCtrlHolder()
{
}
