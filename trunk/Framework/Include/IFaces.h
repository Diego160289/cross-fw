#ifndef __IFACES_H__
#define __IFACES_H__

#define CALL_TYPE __stdcall

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
    virtual unsigned long CALL_TYPE AddRef() = 0;
    virtual unsigned long CALL_TYPE Release() = 0;
    virtual bool CALL_TYPE QueryInterface(const char *ifaceId, void **iface, IErrorInfo *errInfo = 0) = 0;
  };

  struct IErrorInfo
    : public IBase
  {
    DECLARE_UUID(0f3cec41-f5d0-4ec0-abf2-061d1e61c950)
    virtual RetCode CALL_TYPE AddError(unsigned long code, const char *message) = 0;
    virtual RetCode CALL_TYPE AddError(unsigned long code, const char *message, const char *file, unsigned long line) = 0;
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
      vtString, vtWString
    };
    virtual RetCode CALL_TYPE SetValue(VariantType type, const void *value) = 0;
    virtual RetCode CALL_TYPE SetBinaryValue(const void *value, unsigned long bytes) = 0;
    virtual RetCode CALL_TYPE GetValue(void **value) const = 0;
    virtual unsigned long CALL_TYPE GetValueSize() const = 0;
    virtual bool CALL_TYPE IsEmpty() const = 0;
    virtual VariantType CALL_TYPE GetType() const = 0;
    virtual RetCode CALL_TYPE Clear() = 0;
    virtual const char* CALL_TYPE PackToBase64() const = 0;
    virtual RetCode CALL_TYPE FromBase64Pack(const char *pkg) = 0;
  };

  struct INamedVariable
    : public IBase
  {
    DECLARE_UUID(e6dcbc5f-15c3-4513-9e0f-51011ed23536)
    virtual const char* CALL_TYPE GetName() const = 0;
    virtual RetCode CALL_TYPE Get(IVariant **var) const = 0;
    virtual RetCode CALL_TYPE Set(IVariant *var) = 0;
  };

  struct IEnum
    : public IBase
  {
    DECLARE_UUID(327f4f3c-f36d-4168-940d-f690cbb6c65c)
    virtual RetCode CALL_TYPE First() = 0;
    virtual RetCode CALL_TYPE Next(IBase **item) = 0;
    virtual RetCode CALL_TYPE Clone(IEnum **newEnum) const= 0;
  };

  struct IRegistryCtrl
    : public IBase
  {
    DECLARE_UUID(2bc18fdd-9f72-482b-887d-f6e02e930b86)
    virtual RetCode CALL_TYPE Create(const char *registryPath) = 0;
    virtual RetCode CALL_TYPE Remove(const char *registryPath) = 0;
    virtual RetCode CALL_TYPE Load(const char *registryPath) = 0;
    virtual bool CALL_TYPE IsLoaded() const = 0;
    virtual RetCode CALL_TYPE Unload() = 0;
    virtual RetCode CALL_TYPE Save() = 0;
    virtual bool CALL_TYPE IsModified() = 0;
    virtual const char* CALL_TYPE GetCtrlVersion() const = 0;
    virtual const char* CALL_TYPE GetLoadedRegistryVersion() const = 0;
  };

  struct IRegistry
    : public IBase
  {
    DECLARE_UUID(c5d042c6-e6ec-4361-99b8-78026611b87b)
    virtual RetCode CALL_TYPE CreateKey(const char *pathKey) = 0;
    virtual RetCode CALL_TYPE RemoveKey(const char *pathKey) = 0;
    virtual RetCode CALL_TYPE GetValue(const char *pathKey, IVariant **value) = 0;
    virtual RetCode CALL_TYPE SetValue(const char *pathKey, IVariant *value) = 0;
    virtual RetCode CALL_TYPE EnumKey(const char *pathKey, IEnum **enumKey) = 0;
  };

  struct IMessageQueue
    : public IBase
  {
    DECLARE_UUID(cb93b88d-a610-4f9a-9a46-9b63b6935ff1)
    virtual const char* CALL_TYPE GetQueueId() const = 0;
    virtual bool CALL_TYPE SetCallback(IBase *callback) = 0;
    virtual bool CALL_TYPE Post(/*параметры пока не придумал...*/) = 0;
  };

  struct IMessageQueueManager
    : public IBase
  {
    DECLARE_UUID(273689f4-69c7-406e-81dd-5b16684707e2)
    virtual const char* CALL_TYPE CreateMessageQueue() = 0;
    virtual bool CALL_TYPE DestroyMessageQueue(const char *queueId) = 0;
    virtual bool CALL_TYPE QueryMessageQueue(const char *queueId, IMessageQueue **messageQueue) = 0;
    virtual bool CALL_TYPE EnumMessageQueues(IEnum **messageQuques) = 0;
  };

}

#endif	// !__IFACES_H__
