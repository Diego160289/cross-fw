#ifndef __IFACES_H__
#define __IFACES_H__

#define DECLARE_UUID(id_) \
	static const char* GetUUID() \
	{ \
		return #id_ ; \
	}


namespace IFaces
{

  enum RetCode
  {
    retOk, retFalse, retFail,
    retNoInterface, retNotImpl,
    retBadParam
  };

  struct IBase
  {
    DECLARE_UUID(008b199f-09ed-426d-a881-8b8da011640c)
    virtual ~IBase()
    {
    }
    virtual unsigned long AddRef() = 0;
    virtual unsigned long Release() = 0;
    virtual RetCode QueryInterface(const char *ifaceId, void **iface) = 0;
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
      vtString, vtWString
    };
    virtual RetCode SetValue(VariantType type, const void *value) = 0;
    virtual RetCode SetBinaryValue(const void *value, unsigned long bytes) = 0;
    virtual RetCode GetValue(void **value) const = 0;
    virtual unsigned long GetValueSize() const = 0;
    virtual bool IsEmpty() const = 0;
    virtual VariantType GetType() const = 0;
    virtual RetCode Clear() = 0;
    virtual const char* PackToBase64() const = 0;
    virtual RetCode FromBase64Pack(const char *pkg) = 0;
  };

  struct INamedVariable
    : public IBase
  {
    DECLARE_UUID(e6dcbc5f-15c3-4513-9e0f-51011ed23536)
    virtual const char* GetName() const = 0;
    virtual RetCode Get(IVariant **var) const = 0;
    virtual RetCode Set(IVariant *var) = 0;
  };

  struct IEnum
    : public IBase
  {
    DECLARE_UUID(327f4f3c-f36d-4168-940d-f690cbb6c65c)
    virtual RetCode First() = 0;
    virtual RetCode Next(IBase **item) = 0;
    virtual RetCode Clone(IEnum **newEnum) const= 0;
  };

  struct IVarMap
    : public IBase
  {
    DECLARE_UUID(6a427591-c9b1-45f0-b676-b940479e14d8)
    virtual RetCode AddVariable(INamedVariable *namedVar) = 0;
    virtual RetCode RemoveVariable(const char *varName) = 0;
    virtual RetCode GetVariable(const char *varName, IVariant **var) const = 0;
    virtual RetCode EnumVariables(IEnum **vars) const = 0;
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
    virtual const char* GetCtrlVersion() const = 0;
  };

  struct IRegistry
    : public IBase
  {
    DECLARE_UUID(c5d042c6-e6ec-4361-99b8-78026611b87b)
    virtual RetCode CreateKey(const char *pathKey) = 0;
    virtual RetCode RemoveKey(const char *pathKey) = 0;
    virtual RetCode GetValue(const char *pathKey, IVariant **value) = 0;
    virtual RetCode SetValue(const char *pathKey, IVariant *value) = 0;
    virtual RetCode EnumKey(const char *pathKey, IEnum **keys) = 0;
  };

  struct IClassFactory
    : public IBase
  {
    DECLARE_UUID(5fc11638-0ef4-4a55-9f04-156c9533cb05)
    virtual RetCode CreateObject(const char *classId, IBase **obj) = 0;
  };

  struct IClassFactoryCtrl
    : public IBase
  {
    DECLARE_UUID(64b0fdea-e929-4852-a858-0bea41dc6a0f)
    virtual RetCode SetRegistry(IRegistry *reg) = 0;
  };

  struct IServiceManager;

  struct IService
    : public IBase
  {
    DECLARE_UUID(2ce86774-4ad9-4db6-ad48-ab65213c6d32)
    virtual RetCode SetParams(IVarMap *params) = 0;
    virtual RetCode Init() = 0;
    virtual void Done() = 0;
    virtual bool CanDone() const = 0;
  };

  struct IServiceManager
    : public IBase
  {
    DECLARE_UUID(cfa6a777-bcd2-4c46-aefb-082d192119d4)
    virtual unsigned long StartService(const char *serviceId, IBase **service) = 0;
    virtual unsigned long StartService(const char *serviceId) = 0;
    virtual RetCode StopService(unsigned long instanceId) = 0;
    virtual RetCode PostStopToService(unsigned long instanceId) = 0;
    virtual RetCode StopServiceGroup(const char *serviceId) = 0;
    virtual RetCode PostStopToServiceGroup(const char *serviceId) = 0;
    virtual RetCode PostStopToServiceManager() = 0;
    virtual RetCode GetServicePool(const char *serviceId, IEnum **services) = 0;
  };

  struct IServiceManagerCtrl
    : public IBase
  {
    DECLARE_UUID(fb6266b9-2fc8-4d1d-b15c-7d9da0134d3d)
    virtual RetCode SetRegistry(IRegistry *registry) = 0;
    virtual RetCode SetClassFactory(IClassFactory *factory) = 0;
    virtual RetCode Run(const char *startServiceId) = 0;
  };

  struct IInputStream
    : public IBase
  {
    DECLARE_UUID(2b9ae082-b59a-4d6a-a229-5e41d472c92d)
  };

  struct IOutputStream
    : public IBase
  {
    DECLARE_UUID(db609d41-e4bd-42fb-9d3a-fd0d91cac762)
  };

  struct ISerialization
    : public IBase
  {
    DECLARE_UUID(d96e9ec2-8ab8-409c-a8cc-0a8785cc76ef)
    virtual RetCode Load(IInputStream *stream) = 0;
    virtual RetCode Save(IOutputStream *stream) = 0;
    virtual const char* GetStreamName() const = 0;
  };

  struct IProperties
    : public IBase
  {
    DECLARE_UUID(1b209ba5-2071-465a-adda-ea6b4721c4be)
    virtual RetCode AddProperty(INamedVariable *prop) = 0;
    virtual RetCode RemoveProperty(const char *propName) = 0;
    virtual RetCode GetValue(const char *propName, IVariant **value) const = 0;
    virtual RetCode SetValue(const char *propName, IVariant *value) = 0;
    virtual RetCode EnumProperties(IEnum **props) const = 0;
  };

  struct IPersistsProperties
    : public IProperties
    , public ISerialization
  {
    DECLARE_UUID(9f8dc2a8-c0ec-4753-934c-536e493ef6ff)
  };

  struct ILogObject
    : public IBase
  {
    DECLARE_UUID(29578a3f-8f5e-4613-b3a9-37d05caf2c0a)
  };

  struct IEnvironment
    : public IBase
  {
    DECLARE_UUID(4e3ef6a3-bd77-4ff9-a33f-bc9ff342bedf)
    virtual RetCode EnumEnvironmentIFacesId(IEnum **ifaceIds) const = 0;
    virtual RetCode GetInterface(const char *ifaceId, IBase **iface) const = 0;
    virtual RetCode SetInterface(const char *ifaceId, IBase *iface) = 0;
  };

  struct IConfig
    : public IBase
  {
    DECLARE_UUID(a06ead08-0c89-4aed-aafd-885786434ae9)
    virtual RetCode GetValue(const char *name, IVariant **value) const = 0;
    virtual RetCode EnumValues(IEnum **values) const = 0;
  };

  struct IRawDataBuffer
    : public IBase
  {
    DECLARE_UUID(2d96dd96-6f3e-4be2-a579-8f84cacf12c5)
    virtual void* GetData() = 0;
    virtual const void* GetData() const = 0;
    virtual unsigned long GetSize() const = 0;
  };

  struct IStream
    : public IBase
  {
    DECLARE_UUID(757dba03-6669-4204-8303-b5f016c621b5)
    virtual RetCode GetSize(unsigned long *size) const = 0;
    virtual RetCode Read(void *buf, unsigned long bufSize, unsigned long *readBytes) = 0;
    virtual RetCode Write(const void *buf, unsigned long bytes) = 0;
    virtual RetCode SeekToBegin() = 0;
    virtual RetCode SeekToEnd() = 0;
    virtual RetCode SeekTo(unsigned long pos) = 0;
    virtual RetCode GetPos(unsigned long *pos) const = 0;
    virtual RetCode CopyTo(IStream *dest) const = 0;
  };

  struct IStorage
    : public IBase
  {
    DECLARE_UUID(1f1670a1-b85b-4d99-b369-e55e1cf05fc4)
    virtual RetCode CreateStorage(const char *name, IStorage **storage) = 0;
    virtual RetCode OpenStorage(const char *name, IStorage **storage) = 0;
    virtual RetCode CreateStream(const char *name, IStream **stream) = 0;
    virtual RetCode OpenStream(const char *name, IStream **stream) = 0;
    virtual RetCode RemoveItem(const char *name) = 0;
    virtual RetCode Enum(IEnum **items) const = 0;
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


  // TODO: удалить/ это для теста
  struct ITestService
    : public IBase
  {
    DECLARE_UUID(ede23db2-b813-4360-8aa5-d43d8c3c40aa)
    virtual void Test() = 0;
  };
  struct ITestService1
    : public IBase
  {
    DECLARE_UUID(71a5fec2-7afe-4199-8f14-71189770b4a3)
    virtual void Test() = 0;
  };
  struct ITestService2
    : public IBase
  {
    DECLARE_UUID(9c152f52-60ab-44ca-9c54-62cccc496c37)
    virtual void Test() = 0;
  };

}

#endif	// !__IFACES_H__
