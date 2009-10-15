#include "Module.h"
#include "TestService1.h"
#include "Mutex.h"

DECLARE_MODULE_ENTRY_POINT(
    "TestService1",
    dd753419-90e2-448e-b500-093974026ba6,
    System::Mutex,
    TYPE_LIST_1(ITestService1Impl))
