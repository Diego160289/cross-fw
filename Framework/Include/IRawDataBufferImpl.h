#ifndef __IRAWDATABUFFER_H__
#define __IRAWDATABUFFER_H__

#include "IFacesTools.h"
#include "Exceptions.h"
#include "Typedefs.h"

namespace IFacesImpl
{

  using IFaces::RetCode;
  using IFaces::retOk;
  using IFaces::retFalse;
  using IFaces::retFail;
  using IFaces::retBadParam;

  class IRawDataBufferImpl
    : public Common::CoClassBase
        <
          TYPE_LIST_1(IFaces::IRawDataBuffer)
        >
  {
  public:
    DECLARE_UUID(f8bf2caf-0f3e-4f26-8f83-8e9c1cecc84c)

    IRawDataBufferImpl();
    virtual ~IRawDataBufferImpl();

    // IRawDataBuffer
    virtual void* GetData();
    virtual const void* GetData() const;
    virtual unsigned long GetSize() const;

    void SetData(const void *data, unsigned long bytes);
    void AppendData(const void *data, unsigned long bytes);
    void Resize(unsigned long newSize);

  private:
    Common::CharVector Buf;
  };

}

#endif // !__IRAWDATABUFFER_H__
