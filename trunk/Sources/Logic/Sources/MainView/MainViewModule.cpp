#include "../../Framework/Include/Module.h"
#include "../../Framework/Include/Mutex.h"
#include "MainView.h"


DECLARE_MODULE_ENTRY_POINT(
    "MainView",
    665880c7-643b-484e-9549-fc788706acdf,
    System::Mutex,
    TYPE_LIST_1(IMainViewImpl))
