#include "../../Framework/Include/Module.h"
#include "../../Framework/Include/Mutex.h"
#include "MainService.h"


DECLARE_MODULE_ENTRY_POINT(
    "MainService",
    e2a58eae-d8bc-4e7b-a591-6eb827ea96ff,
    System::Mutex,
    TYPE_LIST_1(IMainServiceImpl))
