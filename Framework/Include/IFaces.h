#ifndef __IFACES_H__
#define __IFACES_H__

#define INTERFACE_UUID(id_) \
	static const char* GetIFaceId() \
	{ \
		return #id_ ; \
	}

namespace IFaces
{
  struct IErrorInfo;

  enum RetCode
  {
    retOk, retFail
  };

  struct IBase
  {
    INTERFACE_UUID(008b199f-09ed-426d-a881-8b8da011640c)
    virtual ~IBase()
    {
    }
    virtual unsigned long AddRef() = 0;
    virtual unsigned long Release() = 0;
    virtual bool QueryInterface(const char *ifaceId, void **iface, IErrorInfo *errInfo = 0) = 0;
  };

  struct IErrorInfo
    : public IBase
  {
    INTERFACE_UUID(0f3cec41-f5d0-4ec0-abf2-061d1e61c950)
    virtual RetCode AddError(unsigned long code, const char *message) = 0;
    virtual RetCode AddError(unsigned long code, const char *message, const char *file, unsigned long line) = 0;
  };

  struct IRegistry
    : public IBase
  {
    INTERFACE_UUID(c5d042c6-e6ec-4361-99b8-78026611b87b)
    virtual RetCode Open(const char *registryPath) = 0;
    virtual RetCode Close() = 0;
    virtual RetCode IsOpen() = 0;
  };

}

#endif	// !__IFACES_H__
