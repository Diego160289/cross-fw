#ifndef __IFUNCTIONIMPL_H__
#define __IFUNCTIONIMPL_H__

#include "IFacesTools.h"
#include "Exceptions.h"
#include "Xml/XmlTools.h"

namespace IFacesImpl
{

  using IFaces::RetCode;
  using IFaces::retOk;
  using IFaces::retFalse;
  using IFaces::retFail;
  using IFaces::retNoInterface;
  using IFaces::retBadParam;

  DECLARE_LOGIC_EXCEPTION(IPropertyArgumentImpl)

  class IPropertyArgumentImpl
    : public Common::CoClassBase
        <
          TYPE_LIST_1(IFaces::IPropertyArgument)
        >
  {
  public:
    DECLARE_UUID(b16bb225-a339-4c20-9fc4-d96ee023676c)

    // IPropertyArgument
    virtual const char* GetName() const;
    virtual RetCode GetValue(IFaces::IVariant **value) const;
    virtual RetCode GetObject(IFaces::IObjectArgument **obj) const;
    virtual RetCode GetArray(IFaces::IArrayArgument **arr) const;

    void SetName(const std::string &name);
    void SetObject(Common::RefObjPtr<IFaces::IObjectArgument> obj);
    void SetArray(Common::RefObjPtr<IFaces::IArrayArgument> arr);
    void SetValue(Common::RefObjPtr<IFaces::IVariant> val);
  private:
    std::string Name;
    mutable Common::RefObjPtr<IFaces::IObjectArgument> Obj;
    mutable Common::RefObjPtr<IFaces::IArrayArgument> Arr;
    mutable Common::RefObjPtr<IFaces::IVariant> Val;
  };


  DECLARE_LOGIC_EXCEPTION(IArrayArgumentImpl)

  class IArrayArgumentImpl
    : public Common::CoClassBase
        <
          TYPE_LIST_1(IFaces::IArrayArgument)
        >
  {
  public:
    DECLARE_UUID(49e0bf29-161e-4451-ac4a-ed87501d75ee)

    // IArrayArgument
    virtual unsigned GetCount() const;
    virtual RetCode GetItem(unsigned index, IFaces::IPropertyArgument **item) const;

    void AddProperty(Common::RefObjPtr<IFaces::IPropertyArgument> prop);
  private:
    typedef std::vector<Common::RefObjPtr<IFaces::IPropertyArgument> > PropPool;
    PropPool Props;
  };


  DECLARE_LOGIC_EXCEPTION(IObjectArgumentImpl)

  class IObjectArgumentImpl
    : public Common::CoClassBase
        <
          TYPE_LIST_1(IFaces::IObjectArgument)
        >
  {
  public:
    DECLARE_UUID(21a9c8fa-59a7-4e58-859c-000b6535600c)

    // IObjectArgument
    virtual RetCode GetProperty(const char *propName, IFaces::IPropertyArgument **prop) const;
    virtual RetCode GetArray(IFaces::IArrayArgument **arr) const;

    void AddProperty(Common::RefObjPtr<IFaces::IPropertyArgument> prop);
    void SetArray(Common::RefObjPtr<IFaces::IArrayArgument> arr);
  private:
    typedef std::vector<Common::RefObjPtr<IFaces::IPropertyArgument> > PropPool;
    mutable PropPool Props;
    mutable Common::RefObjPtr<IFaces::IArrayArgument> Arr;
  };


  class IFunctionImpl
    : public Common::CoClassBase
        <
          TYPE_LIST_1(IFaces::IFunction)
        >
  {
  public:
    DECLARE_UUID(37142bc6-e95f-450d-980c-4d3b30ced38b)

    // IFunction
    virtual const char* GetFunctionName() const;
    virtual RetCode GetValue(IFaces::IVariant **value) const;
    virtual RetCode GetObject(IFaces::IObjectArgument **obj) const;
    virtual RetCode GetArray(IFaces::IArrayArgument **arr) const;

    void SetFunctionName(const std::string &name);
    void SetObject(Common::RefObjPtr<IFaces::IObjectArgument> obj);
    void SetArray(Common::RefObjPtr<IFaces::IArrayArgument> arr);
    void SetValue(Common::RefObjPtr<IFaces::IVariant> val);
  private:
    std::string FunctionName;
  };

  Common::RefObjPtr<IFaces::IFunction>
    FunctionFromNode(const Common::XmlTools::Node &node, Common::ISynObj &syn);

  template <typename TSyn>
  Common::RefObjPtr<IFaces::IFunction>
    FunctionFromNode(const Common::XmlTools::Node &node)
  {
    return FunctionFromNode(node, Common::ISynObjImpl<TSyn>());
  }

}

#endif  // !__IFUNCTIONIMPL_H__
