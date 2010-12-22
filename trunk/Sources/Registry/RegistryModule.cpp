//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "Module.h"
#include "Registry.h"
#include "Mutex.h"


DECLARE_MODULE_ENTRY_POINT(
    "Registry",
    5631ec27-e1a0-430b-a723-4bf7575f06b5,
    System::Mutex,
    TYPE_LIST_1(IRegistryImpl))

