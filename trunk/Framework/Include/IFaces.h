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
    virtual void AddError(unsigned long code, const char *message) = 0;
  };
}

#endif	// !__IFACES_H__