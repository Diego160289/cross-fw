//============================================================================
// Date        : 05.01.2010
// Author      : Volodin Oleg
// Copyright   : (c) Volodin Oleg (oleg.volodin@perspectsoft.com)
//============================================================================


#include "Module.h"
#include "LoggerImpl.h"
#include "Mutex.h"


DECLARE_MODULE_ENTRY_POINT(
    "Logger",
    59ee24fb-b997-4456-8bf5-952b3613bfab,
    System::Mutex,
    TYPE_LIST_1(ILoggerImpl))
