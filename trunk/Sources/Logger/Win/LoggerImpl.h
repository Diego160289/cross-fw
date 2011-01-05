//============================================================================
// Date        : 05.01.2010
// Author      : Volodin Oleg
// Copyright   : (c) Volodin Oleg (oleg.volodin@perspectsoft.com)
//============================================================================


#ifndef __LOGGERIMPL_H__
#define __LOGGERIMPL_H__

#include "ComponentUUIDs.h"
#include "IFacesTools.h"
#include "Logger.h"
//#include "DBWrap.h"


class ILoggerImpl
  : public Common::CoClassBase
      <
        Common::TypeListAdapter
        <
          IFaces::LoggerIFaces::ILogger,
          IFaces::LoggerIFaces::ILoggerCtrl,
          IFaces::LoggerIFaces::ILoggerFileCtrl
        >
      >
{
public:
  DECLARE_STR_UUID(UUID_LOGGERIMPL)

  // ILogger
  

  virtual bool FinalizeCreate();
  virtual void BeforeDestroy();

private:
  
};

#endif  // !__LOGGERIMPL_H__
