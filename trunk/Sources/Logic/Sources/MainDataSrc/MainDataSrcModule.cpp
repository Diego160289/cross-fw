#include "../../Framework/Include/Module.h"
#include "../../Framework/Include/Mutex.h"
#include "MainDataSrc.h"


DECLARE_MODULE_ENTRY_POINT(
    "MainDataSrc",
    508f7132-acd6-46ef-8249-c215b9364a12,
    System::Mutex,
    TYPE_LIST_1(IMainDataSrcImpl))
