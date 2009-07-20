#include "GenerateUUIDImpl.h"
#include "DllHolder.h"

#include <stdexcept>

#include <stdlib.h>

#include "uuid.h"

namespace System
{

  std::string GenerateUUIDImpl()
  {
    uuid_t *Uuid = 0;
    if (uuid_create(&Uuid) != UUID_RC_OK)
      throw std::runtime_error("Can't create UUID");
    std::string RetUuid;
    uuid_rc_t RetCode = uuid_make(Uuid, UUID_MAKE_V3);
    if (RetCode == UUID_RC_OK)
    {
      char *Buf = 0;
      size_t Len = 0;
      RetCode = uuid_export(Uuid, UUID_FMT_STR, (void**) &Buf, &Len);
      if (RetCode == UUID_RC_OK)
      {
        RetUuid = Buf;
        free(Buf);
      }
    }
    uuid_destroy(Uuid);
    if (RetCode != UUID_RC_OK)
      throw std::runtime_error("Can't create UUID");
    return RetUuid;
  }

}
