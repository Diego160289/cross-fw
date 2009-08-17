#include "IFunctionImpl.h"
#include "IVariantImpl.h"
#include "RefObjQIPtr.h"
#include "CommonUtils.h"


namespace IFacesImpl
{

  const char* IPropertyArgumentImpl::GetName() const
  {
    Common::ISyncObject Locker(GetSynObj());
    return Name.c_str();
  }

  RetCode IPropertyArgumentImpl::GetValue(IFaces::IVariant **value) const
  {
    Common::ISyncObject Locker(GetSynObj());
    return Val.Get() ? Val.QueryInterface(value) : retNoInterface;
  }

  RetCode IPropertyArgumentImpl::GetObject(IFaces::IObjectArgument **obj) const
  {
    Common::ISyncObject Locker(GetSynObj());
    return Obj.Get() ? Val.QueryInterface(obj) : retNoInterface;
  }

  RetCode IPropertyArgumentImpl::GetArray(IFaces::IArrayArgument **arr) const
  {
    Common::ISyncObject Locker(GetSynObj());
    return Arr.Get() ? Arr.QueryInterface(arr) : retNoInterface;
  }

  void IPropertyArgumentImpl::SetName(const std::string &name)
  {
    Common::ISyncObject Locker(GetSynObj());
    Name = name;
  }

  void IPropertyArgumentImpl::SetObject(Common::RefObjPtr<IFaces::IObjectArgument> obj)
  {
    Common::ISyncObject Locker(GetSynObj());
    if (Val.Get() || Arr.Get())
      throw IPropertyArgumentImplException("Can't set object");
    Obj = obj;
  }

  void IPropertyArgumentImpl::SetArray(Common::RefObjPtr<IFaces::IArrayArgument> arr)
  {
    Common::ISyncObject Locker(GetSynObj());
    if (Val.Get() || Obj.Get())
      throw IPropertyArgumentImplException("Can't set array");
    Arr = arr;
  }

  void IPropertyArgumentImpl::SetValue(Common::RefObjPtr<IFaces::IVariant> val)
  {
    Common::ISyncObject Locker(GetSynObj());
    if (Obj.Get() || Arr.Get())
      throw IPropertyArgumentImplException("Can't set value");
    Val = val;
  }


  unsigned IArrayArgumentImpl::GetCount() const
  {
    Common::ISyncObject Locker(GetSynObj());
    return static_cast<unsigned>(Props.size());
  }

  RetCode IArrayArgumentImpl::GetItem(unsigned index, IFaces::IPropertyArgument **item) const
  {
    Common::ISyncObject Locker(GetSynObj());
    if (index >= static_cast<unsigned>(Props.size()))
      return retBadParam;
    Common::RefObjPtr<IFaces::IPropertyArgument> Prop(Props[index]);
    if (Prop->GetName() != Common::ToString(index))
      return retFail;
    return Prop.QueryInterface(item);
  }

  void IArrayArgumentImpl::AddProperty(Common::RefObjPtr<IFaces::IPropertyArgument> prop)
  {
    Common::ISyncObject Locker(GetSynObj());
    for (PropPool::const_iterator i = Props.begin() ; i != Props.end() ; ++i)
    {
      if ((*i)->GetName() == std::string(prop->GetName()))
        throw IArrayArgumentImplException("Item with such index already exists");
    }
    Props.push_back(prop);
  }


  RetCode IObjectArgumentImpl::GetProperty(const char *propName, IFaces::IPropertyArgument **prop) const
  {
    Common::ISyncObject Locker(GetSynObj());
    if (Arr.Get())
      return retNoInterface;
    for (PropPool::iterator i = Props.begin() ; i != Props.end() ; ++i)
    {
      if ((*i)->GetName() == std::string(propName))
        return i->QueryInterface(prop);
    }
    return retFail;
  }

  RetCode IObjectArgumentImpl::GetArray(IFaces::IArrayArgument **arr) const
  {
    Common::ISyncObject Locker(GetSynObj());
    return Arr.Get() ? Arr.QueryInterface(arr) : retNoInterface;
  }

  void IObjectArgumentImpl::AddProperty(Common::RefObjPtr<IFaces::IPropertyArgument> prop)
  {
    Common::ISyncObject Locker(GetSynObj());
    if (Arr.Get())
      throw IObjectArgumentImplException("Can't add property");
    for (PropPool::const_iterator i = Props.begin() ; i != Props.end() ; ++i)
    {
      if ((*i)->GetName() == std::string(prop->GetName()))
        throw IArrayArgumentImplException("Property with such name already exists");
    }
    Props.push_back(prop);
  }

  void IObjectArgumentImpl::SetArray(Common::RefObjPtr<IFaces::IArrayArgument> arr)
  {
    Common::ISyncObject Locker(GetSynObj());
    if (Props.size())
      throw IObjectArgumentImplException("Can't set array");
  }


  const char* IFunctionImpl::GetFunctionName() const
  {
    Common::ISyncObject Locker(GetSynObj());
    return FunctionName.c_str();
  }

  RetCode IFunctionImpl::GetValue(IFaces::IVariant **value) const
  {
    Common::ISyncObject Locker(GetSynObj());
    return retFail;
  }

  RetCode IFunctionImpl::GetObject(IFaces::IObjectArgument **obj) const\
  {
    Common::ISyncObject Locker(GetSynObj());
    return retFail;
  }

  RetCode IFunctionImpl::GetArray(IFaces::IArrayArgument **arr) const
  {
    Common::ISyncObject Locker(GetSynObj());
    return retFail;
  }

  void IFunctionImpl::SetFunctionName(const std::string &name)
  {
    Common::ISyncObject Locker(GetSynObj());
    FunctionName = name;
  }

  void IFunctionImpl::SetObject(Common::RefObjPtr<IFaces::IObjectArgument> obj)
  {
  }

  void IFunctionImpl::SetArray(Common::RefObjPtr<IFaces::IArrayArgument> arr)\
  {
  }

  void IFunctionImpl::SetValue(Common::RefObjPtr<IFaces::IVariant> val)
  {
  }


  namespace
  {
    DECLARE_LOGIC_EXCEPTION(FunctionCreator)

    class FunctionCreator
    {
    public:
      static Common::RefObjPtr<IFaces::IFunction>
        Create(const Common::XmlTools::Node &node, Common::ISynObj &syn)
      {
        Common::RefObjPtr<IFunctionImpl> Function =
          Common::IBaseImpl<IFunctionImpl>::CreateWithSyn(syn);

        using namespace Common::XmlTools;

        NodeList Root(NodePtr(new Node(node)));
        std::string FunctionName = Root[Tag("invoke")][Attribute("name")][0]->GetPropertiesMap()["name"];
        Function->SetFunctionName(FunctionName);
        NodePtr Args = Root[Tag("invoke")][0]->GetChildNodes()[Tag("arguments")][0];
        if (Args->HasChildList())
        {
          const NodeList &ArgList = Args->GetChildNodes();
          if (ArgList.size() != 1)
            throw FunctionCreatorException("Bad arg list");
          std::string NodeName = (*ArgList.begin())->GetNodeName();
          if (NodeName == "object")
            Function->SetObject(ExtractObject(*ArgList.begin(), syn));
          else
            if (NodeName == "array")
              Function->SetArray(ExtractArray(*ArgList.begin(), syn));
            else
              if (NodeName == "string")
                Function->SetValue(ExtractString(*ArgList.begin(), syn));
              else
                throw FunctionCreatorException("Bad argument");
        }

        return Common::RefObjQIPtr<IFaces::IFunction>(Function);
      }
    private:
      static Common::RefObjQIPtr<IFaces::IObjectArgument>
      ExtractObject(Common::XmlTools::NodePtr node, Common::ISynObj &syn)
      {
        Common::RefObjPtr<IObjectArgumentImpl> NewObject =
          Common::IBaseImpl<IObjectArgumentImpl>::CreateWithSyn(syn);
        using namespace Common::XmlTools;
        const NodeList &List = node->GetChildNodes();
        for (NodeList::const_iterator i = List.begin() ; i != List.end() ; ++i)
        {
          std::string NodeName = (*i)->GetNodeName();
          if (NodeName == "property")
            NewObject->AddProperty(ExtractProperty(*i, syn));
          else
            if (NodeName == "array")
              NewObject->SetArray(ExtractArray(*i, syn));
            else
              throw FunctionCreatorException("Bad object item");
        }
        return NewObject;
      }
      static Common::RefObjQIPtr<IFaces::IPropertyArgument>
      ExtractProperty(Common::XmlTools::NodePtr node, Common::ISynObj &syn)
      {
        Common::RefObjPtr<IPropertyArgumentImpl> NewProperty =
          Common::IBaseImpl<IPropertyArgumentImpl>::CreateWithSyn(syn);
        NewProperty->SetName(node->GetPropertiesMap()["id"]);
        using namespace Common::XmlTools;
        const NodeList &List = node->GetChildNodes();
        for (NodeList::const_iterator i = List.begin() ; i != List.end() ; ++i)
        {
          std::string NodeName = (*i)->GetNodeName();
          if (NodeName == "object")
            NewProperty->SetObject(ExtractObject(*i, syn));
          else
            if (NodeName == "array")
              NewProperty->SetArray(ExtractArray(*i, syn));
            else
              if (NodeName == "string")
                NewProperty->SetValue(ExtractString(*i, syn));
              else
                throw FunctionCreatorException("Bad property item");
        }
        return NewProperty;
      }
      static Common::RefObjQIPtr<IFaces::IArrayArgument>
      ExtractArray(Common::XmlTools::NodePtr node, Common::ISynObj &syn)
      {
        Common::RefObjPtr<IArrayArgumentImpl> NewArray =
          Common::IBaseImpl<IArrayArgumentImpl>::CreateWithSyn(syn);
        using namespace Common::XmlTools;
        const NodeList &List = node->GetChildNodes();
        for (NodeList::const_iterator i = List.begin() ; i != List.end() ; ++i)
        {
          std::string NodeName = (*i)->GetNodeName();
          if (NodeName == "property")
            NewArray->AddProperty(ExtractProperty(*i, syn));
          else
            throw FunctionCreatorException("Bad array item");
        }
        return NewArray;
      }
      static Common::RefObjQIPtr<IFaces::IVariant>
      ExtractString(Common::XmlTools::NodePtr node, Common::ISynObj &syn)
      {
        Common::RefObjPtr<IVariantImpl> NewValue =
          Common::IBaseImpl<IVariantImpl>::CreateWithSyn(syn);
        IVariantHelper Helper(NewValue);
        Helper = node->GetValue().c_str();
        return NewValue;
      }
    };
  }

  Common::RefObjPtr<IFaces::IFunction>
    FunctionFromNode(const Common::XmlTools::Node &node, Common::ISynObj &syn)
  {
    return FunctionCreator::Create(node, syn);
  }

}
