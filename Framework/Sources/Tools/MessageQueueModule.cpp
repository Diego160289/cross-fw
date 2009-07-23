#include "Module.h"
#include "MessageQueue.h"
#include "Mutex.h"

DECLARE_MODULE_ENTRY_POINT(
    "Tools",
    d204eea6-4278-4282-b64c-767fec43d48d,
    System::Mutex,
    TYPE_LIST_1(IMessageQueueManagerImpl))
