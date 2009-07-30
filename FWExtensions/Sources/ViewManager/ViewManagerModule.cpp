#include "../../Framework/Include/Module.h"
#include "../../Framework/Include/Mutex.h"
#include "ViewManager.h"


DECLARE_MODULE_ENTRY_POINT(
    "ViewManager",
    6d00c41b-cf4b-46e1-ac3a-6dbd93a0be6a,
    System::Mutex,
    TYPE_LIST_1(IViewManagerImpl))
