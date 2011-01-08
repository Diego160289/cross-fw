//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __IERRORINFOIMPL_H__
#define __IERRORINFOIMPL_H__

#include "IFacesTools.h"


namespace IFacesImpl
{
  using IFaces::RetCode;
  using IFaces::retOk;
  using IFaces::retFail;

  class IErrorInfoImpl
    : public Common::CoClassBase
        <
          TYPE_LIST_1(IFaces::IErrorInfo)
        >
  {
  public:
    DECLARE_UUID(631e8a5c-9e5f-4ea3-b115-f88d84119923)

    virtual RetCode AddError(unsigned long /*code*/, const char * /*message*/)
    {
      return retOk;
    }
    virtual RetCode AddError(unsigned long /*code*/, const char * /*message*/, const char * /*file*/, unsigned long /*line*/)
    {
      return retOk;
    }
  };
}

#endif	// !__IERRORINFOIMPL_H__
