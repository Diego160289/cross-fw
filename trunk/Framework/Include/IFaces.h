#ifndef __IFACES_H__
#define __IFACES_H__

#define DECLARE_UUID(id_) \
	static const char* GetUUID() \
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
    DECLARE_UUID(008b199f-09ed-426d-a881-8b8da011640c)
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
    DECLARE_UUID(0f3cec41-f5d0-4ec0-abf2-061d1e61c950)
    virtual RetCode AddError(unsigned long code, const char *message) = 0;
    virtual RetCode AddError(unsigned long code, const char *message, const char *file, unsigned long line) = 0;
  };

  struct IVariant
    : public IBase
  {
    DECLARE_UUID(c6f1880a-d666-453c-ae5c-f3c5c30ff526)
  };

  struct IEnum
    : public IBase
  {
    DECLARE_UUID(327f4f3c-f36d-4168-940d-f690cbb6c65c)
  };

  struct IRegistryCtrl
    : public IBase
  {
    DECLARE_UUID(2bc18fdd-9f72-482b-887d-f6e02e930b86)
    virtual RetCode Create(const char *registryPath) = 0;
    virtual RetCode Load(const char *registryPath) = 0;
    virtual RetCode Save(const char *registryPath) = 0;
    virtual RetCode IsModified() = 0;
    virtual RetCode Close() = 0;
  };

  struct IRegistry
    : public IBase
  {
    DECLARE_UUID(c5d042c6-e6ec-4361-99b8-78026611b87b)
    virtual RetCode CreatePathKey(const char *pathKey) = 0;
    virtual RetCode GetValue(const char *pathKey, IVariant **value) = 0;
    virtual RetCode SetValue(const char *pathKey, IVariant *value) = 0;
    virtual RetCode EnumKey(const char *pathKey, IEnum **enumKey) = 0;
  };

}

#endif	// !__IFACES_H__
