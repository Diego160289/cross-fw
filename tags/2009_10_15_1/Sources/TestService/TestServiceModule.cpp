#include "Module.h"
#include "TestService.h"
#include "Mutex.h"

DECLARE_MODULE_ENTRY_POINT(
    "TestService",
    d9e520fc-ab19-4a49-aa10-dee583bb7dd5,
    System::Mutex,
    TYPE_LIST_1(ITestServiceImpl))
