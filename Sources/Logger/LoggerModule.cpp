//============================================================================
// Date        : 05.01.2010
// Author      : Volodin Oleg
// Copyright   : (c) Volodin Oleg (oleg.volodin@perspectsoft.com)
//============================================================================


#include "Module.h"
#include "ILogImpl.h"
#include "IStorageImpl.h"
#include "Mutex.h"


typedef Common::TypeListAdapter
  <
    ILogImpl,
    IStorageImpl
  >
  ExportedTypes;

DECLARE_MODULE_ENTRY_POINT
(
  "Logger",
  59ee24fb-b997-4456-8bf5-952b3613bfab,
  System::Mutex,
  ExportedTypes
)
