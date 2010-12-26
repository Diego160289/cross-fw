//============================================================================
// Date        : 26.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#include "Module.h"
#include "TCPClient.h"
#include "Mutex.h"


typedef Common::TypeListAdapter
  <
    ITCPClientImpl
  >
  ExportedTypes;

DECLARE_MODULE_ENTRY_POINT(
    "Network",
    1f687ce0-37e8-4524-8e57-b6908a7de1d3,
    System::Mutex,
    ExportedTypes)
