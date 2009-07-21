#ifndef __PROPERTIESIMPL_H__
#define  __PROPERTIESIMPL_H__

#include "IFacesTools.h"
#include "SyncObj.h"
#include "Exceptions.h"
#include "INamedVariableImpl.h"

namespace IFacesImpl
{

  using IFaces::RetCode;
  using IFaces::retOk;
  using IFaces::retFalse;
  using IFaces::retFail;

  template
  <
    typename TBaseIFace = IFaces::IProperties,
    typename TSynObj = System::MutexStub
  >
  class IProperties
    : public TBaseIFace
  {
  public:
    IProperties()
      : SynObj(0)
    {
    }

    // IProperties
    virtual RetCode AddProperty(IFaces::INamedVariable *prop)
    {
      return retFail;
    }
    virtual RetCode RemoveProperty(const char *propName)
    {
      return retFail;
    }
    virtual RetCode GetValue(const char *propName, IFaces::IVariant **value) const
    {
      return retFail;
    }
    virtual RetCode SetValue(const char *propName, IFaces::IVariant *value)
    {
      return retFail;
    }
    virtual RetCode EnumProperties(IFaces::IEnum **props) const
    {
      return retFail;
    }

  private:
    TSynObj *SynObj;
  protected:
    void InitSynObj(TSynObj *synObj)
    {
      SynObj = synObj;
    }
  };

  template
  <
    typename TSynObj = System::MutexStub
  >
  class IPropertiesImpl
    : public Common::CoClassBase
        <
          IPropertiesImpl<TSynObj>,
          Common::TypeList<IProperties<IFaces::IProperties, TSynObj>, Common::NullType>,
          Common::MultiObject,
          TSynObj
        >
  {
  public:
    DECLARE_UUID(25a525ca-e9b1-4b4b-bb7f-0b4e7e66bba9)

    IPropertiesImpl()
    {
      InitSynObj(&this->GetSynObj());
    }

    typedef IPropertiesImpl<TSynObj> ThisType;
    typedef Common::RefObjPtr<ThisType> ThisTypePtr;
  };


  template
  <
    typename TSynObj = System::MutexStub
  >
  class IPersistsPropertiesImpl
    : public Common::CoClassBase
        <
          IPersistsPropertiesImpl<TSynObj>,
          Common::TypeList<IProperties<IFaces::IPersistsProperties, TSynObj>, Common::NullType>,
          Common::MultiObject, TSynObj
        >
  {
  public:
    DECLARE_UUID(672066ad-3284-474c-bb0b-9bd3ea7302f9)

    typedef IPersistsPropertiesImpl<TSynObj> ThisType;
    typedef Common::RefObjPtr<ThisType> ThisTypePtr;

    // IPersistsProperties
    virtual RetCode Load(IFaces::IInputStream *stream)
    {
      return retFail;
    }
    virtual RetCode Save(IFaces::IOutputStream *stream)
    {
      return retFail;
    }
    virtual const char* GetStreamName() const
    {
      return "";
    }

  };
}

#endif // !__PROPERTIESIMPL_H__
