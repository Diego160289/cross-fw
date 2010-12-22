//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "Module.h"
#include "ServiceManager.h"
#include "Mutex.h"

DECLARE_MODULE_ENTRY_POINT(
    "ServiceManager",
    e5645c37-7dad-434e-bc08-e156f946d943,
    System::Mutex,
    TYPE_LIST_1(IServiceManagerImpl))
