#include "SystemUtils.h"
#include "GenerateUUIDImpl.h"

namespace System
{
  std::string GenerateUUID()
  {
    return GenerateUUIDImpl();
  }
}
