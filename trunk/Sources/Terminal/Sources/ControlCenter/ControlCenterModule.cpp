#include "../../Framework/Include/Module.h"
#include "../../Framework/Include/Mutex.h"
#include "ControlCenter.h"


DECLARE_MODULE_ENTRY_POINT(
    "ControlCenter",
    303d7bb0-074c-41b5-a7ce-75e965223915,
    System::Mutex,
    TYPE_LIST_1(IControlCenterImpl))
