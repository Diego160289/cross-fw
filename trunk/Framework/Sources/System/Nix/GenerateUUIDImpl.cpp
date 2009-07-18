#include "GenerateUUIDImpl.h"
#include "DllHolder.h"

#include <stdexcept>

#include <sstream>

namespace System
{

  std::string GenerateUUIDImpl()
  {
    static unsigned long Index = 0;
    std::stringstream Io;
    Io << "UUID_" << Index++;
    return Io.str();
  }

}
