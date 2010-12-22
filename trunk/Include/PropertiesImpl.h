//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


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
    typename TBaseIFace = IFaces::IProperties
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
    Common::ISynObj *SynObj;
  protected:
    void InitSynObj(Common::ISynObj *synObj)
    {
      SynObj = synObj;
    }
  };

  class IPropertiesImpl
    : public Common::CoClassBase
        <
          TYPE_LIST_1(IProperties<IFaces::IProperties>)
        >
  {
  public:
    DECLARE_UUID(25a525ca-e9b1-4b4b-bb7f-0b4e7e66bba9)

    IPropertiesImpl()
    {
      //InitSynObj(&GetSynObj());
    }
  };


  class IPersistsPropertiesImpl
    : public Common::CoClassBase
        <
          TYPE_LIST_1(IProperties<IFaces::IPersistsProperties>)
        >
  {
  public:
    DECLARE_UUID(672066ad-3284-474c-bb0b-9bd3ea7302f9)

    IPersistsPropertiesImpl()
    {
      //InitSynObj(&GetSynObj());
    }

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
