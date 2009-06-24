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
    retOk, retFalse, retFail
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
    enum VariantType
    {
      vtUnknown,
      vtBool,
      vtBinary,
      vtIBase,
      vtChar, vtUChar,
      vtShort, vtUShort,
      vtInt, vtUInt,
      vtLong, vtULong,
      vtFloat, vtDouble,
      vtConstCharPtr, vtConstWCharPtr
    };
    virtual RetCode SetValue(VariantType type, const void *value) = 0;
    virtual RetCode SetBinaryValue(const void *value, unsigned long bytes) = 0;
    virtual const void* GetValue(VariantType type) const = 0;
    virtual const void* GetBinaryValue() const = 0;
    virtual unsigned long GetValueSize() const = 0;
    virtual RetCode ConvertTo(VariantType type) = 0;
    virtual bool IsEmpty() const = 0;
    virtual VariantType GetType() const = 0;
    virtual RetCode Clear() = 0;
  };

  struct IEnum
    : public IBase
  {
    DECLARE_UUID(327f4f3c-f36d-4168-940d-f690cbb6c65c)
    virtual RetCode First() = 0;
    virtual RetCode Next(IBase **item) = 0;
    virtual RetCode Clone(IEnum **newEnum) const= 0;
  };

  struct IRegistryCtrl
    : public IBase
  {
    DECLARE_UUID(2bc18fdd-9f72-482b-887d-f6e02e930b86)
    virtual RetCode Create(const char *registryPath) = 0;
    virtual RetCode Remove(const char *registryPath) = 0;
    virtual RetCode Load(const char *registryPath) = 0;
    virtual bool IsLoaded() const = 0;
    virtual RetCode Unload() = 0;
    virtual RetCode Save(const char *registryPath) = 0;
    virtual RetCode IsModified() = 0;
    virtual RetCode Close() = 0;
    virtual const char* GetCtrlVersion() const = 0;
    virtual const char* GetLoadedRegistryVersion() const = 0;
  };

  struct IRegistry
    : public IBase
  {
    DECLARE_UUID(c5d042c6-e6ec-4361-99b8-78026611b87b)
    virtual RetCode CreatePathKey(const char *pathKey) = 0;
    virtual RetCode RemovePathKey(const char *pathKey) = 0;
    virtual RetCode GetValue(const char *pathKey, IVariant **value) = 0;
    virtual RetCode SetValue(const char *pathKey, IVariant *value) = 0;
    virtual RetCode EnumKey(const char *pathKey, IEnum **enumKey) = 0;
  };

  struct IMessageQueue
    : public IBase
  {
    DECLARE_UUID(cb93b88d-a610-4f9a-9a46-9b63b6935ff1)
    virtual const char* GetQueueId() const = 0;
    virtual bool SetCallback(IBase *callback) = 0;
    virtual bool Post(/*��������� ���� �� ��������...*/) = 0;
  };

  struct IMessageQueueManager
    : public IBase
  {
    DECLARE_UUID(273689f4-69c7-406e-81dd-5b16684707e2)
    virtual const char* CreateMessageQueue() = 0;
    virtual bool DestroyMessageQueue(const char *queueId) = 0;
    virtual bool QueryMessageQueue(const char *queueId, IMessageQueue **messageQueue) = 0;
    virtual bool EnumMessageQueues(IEnum **messageQuques) = 0;
  };

}

#endif	// !__IFACES_H__
