#include "../../Framework/Include/Module.h"
#include "../../Framework/Include/Mutex.h"
#include "FlashView.h"


DECLARE_MODULE_ENTRY_POINT(
    "FlashView",
    830fe51a-31eb-4104-a411-549fd76d0459,
    System::Mutex,
    TYPE_LIST_1(IFlashViewImpl))
