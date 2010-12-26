//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "Module.h"
#include "DBConnection.h"
#include "Mutex.h"


DECLARE_MODULE_ENTRY_POINT(
    "DBClient",
    5d9e09a6-3285-4f03-a99d-aa0f8a700f7b,
    System::Mutex,
    TYPE_LIST_1(IConnectionImpl))
