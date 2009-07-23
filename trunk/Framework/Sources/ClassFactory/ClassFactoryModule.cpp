#include "Module.h"
#include "ClassFactory.h"
#include "Mutex.h"

DECLARE_MODULE_ENTRY_POINT(
    "ClassFactory",
    28d24a57-9532-41a7-b3fd-67668bcca774,
    System::Mutex,
    TYPE_LIST_1(IClassFactoryImpl))
