#include "IFunctionImpl.h"


namespace IFacesImpl
{

  const char* IPropertyArgumentImpl::GetName() const
  {
    return "";
  }

  RetCode IPropertyArgumentImpl::GetValue(IFaces::IVariant **value) const
  {
    return retFail;
  }

  RetCode IPropertyArgumentImpl::GetObject(IFaces::IObjectArgument **obj) const
  {
    return retFail;
  }

  RetCode IPropertyArgumentImpl::GetArray(IFaces::IArrayArgument **arr) const
  {
    return retFail;
  }


  unsigned IArrayArgumentImpl::GetCount() const
  {
    return 0;
  }

  RetCode IArrayArgumentImpl::GetItem(unsigned index, IFaces::IPropertyArgument **item) const
  {
    return retFail;
  }


  RetCode IObjectArgumentImpl::GetProperty(const char *propName, IFaces::IPropertyArgument **prop) const
  {
    return retFail;
  }

  RetCode IObjectArgumentImpl::GetArray(IFaces::IArrayArgument **arr) const
  {
    return retFail;
  }


  const char* IFunctionImpl::GetFunctionName() const
  {
    return "";
  }

  RetCode IFunctionImpl::GetValue(IFaces::IVariant **value) const
  {
    return retFail;
  }

  RetCode IFunctionImpl::GetObject(IFaces::IObjectArgument **obj) const\
  {
    return retFail;
  }

  RetCode IFunctionImpl::GetArray(IFaces::IArrayArgument **arr) const
  {
    return retFail;
  }

  Common::RefObjPtr<IFaces::IFunction>
    FunctionFromNode(const Common::XmlTools::Node &node, Common::ISynObj &syn)
  {
    return Common::RefObjPtr<IFaces::IFunction>(0);
  }

}
