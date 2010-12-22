//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __ISTREAMHELPER_H__
#define __ISTREAMHELPER_H__

#include "IFaces.h"
#include "RefObjPtr.h"
#include "Exceptions.h"


namespace IFacesImpl
{

  using IFaces::RetCode;
  using IFaces::retOk;


  DECLARE_RUNTIME_EXCEPTION(IStreamHelper)

  class IStreamHelper
  {
  public:
    typedef Common::RefObjPtr<IFaces::IStream> IStreamPtr;

    IStreamHelper(IStreamPtr stream);

    unsigned long GetSize() const;
    unsigned long Read(void *buf, unsigned long bufSize);
    void Write(const void *buf, unsigned long bytes);
    void SeekToBegin();
    void SeekToEnd();
    void SeekTo(unsigned long pos);
    unsigned long GetPos() const;
    void CopyTo(IStreamPtr dest) const;

  private :
    IStreamPtr Stream;
  };

}

#endif // !__ISTREAMHELPER_H__
