#include "Xml/Commands.h"

#include <sstream>


namespace Common
{
  namespace Commands
  {

    IArgument::~IArgument()
    {
    }

    void IArgument::Add(IArgumentPtr arg)
    {
      if (!Arguments.Get())
        Arguments.Reset(new IArgumentList);
      Arguments->push_back(arg);
    }

    IArgumentListIter IArgument::Begin()
    {
      if (!Arguments.Get())
        throw IArgumentException("Empty argument");
      return Arguments->begin();
    }

    IArgumentListCIter IArgument::Begin() const
    {
      if (!Arguments.Get())
        throw IArgumentException("Empty argument");
      return Arguments->begin();
    }

    IArgumentListIter IArgument::End()
    {
      if (!Arguments.Get())
        throw IArgumentException("Empty argument");
      return Arguments->end();
    }

    IArgumentListCIter IArgument::End() const
    {
      if (!Arguments.Get())
        throw IArgumentException("Empty argument");
      return Arguments->end();
    }

    unsigned IArgument::GetCount() const
    {
      if (!Arguments.Get())
        return 0;
      return static_cast<unsigned>(Arguments->size());
    }

    IArgumentPropertyList::IArgumentPropertyList(const IArgument *arg)
      : Arg(arg)
    {
    }

    IArgumentPropertyList::IArgumentPropertyList(const IArgumentPropertyList &lst)
      : Arg(0)
    {
      CopyArg.Reset(new IArgument(*lst.Arg));
      Arg = CopyArg.Get();
    }

    IArgumentPropertyList& IArgumentPropertyList::operator = (const IArgumentPropertyList &lst)
    {
      if (this == &lst)
        return *this;
      Arg = 0;
      CopyArg.Reset(lst.Arg ? new IArgument(*lst.Arg) : 0);
      Arg = CopyArg.Get();
      return *this;
    }

    const IArgumentProperty& IArgumentPropertyList::operator [] (const std::wstring &name) const
    {
      if (Arg)
      {
        for (IArgumentListCIter i = Arg->Begin() ; i != Arg->End() ; ++i)
        {
          const IArgumentProperty &Prop = dynamic_cast<const IArgumentProperty &>(*(i->Get()));
          if (Prop.GetName() == name)
            return Prop;
        }
      }
      throw IArgumentPropertyListException("Property not found");
    }

    const IArgumentProperty& IArgumentPropertyList::operator [] (unsigned index) const
    {
      if (Arg)
      {
        unsigned Index = 0;
        for (IArgumentListCIter i = Arg->Begin() ; i != Arg->End() ; ++i)
        {
          if (Index++ == index)
            return dynamic_cast<const IArgumentProperty &>(*(i->Get()));
        }
      }
      throw IArgumentPropertyListException("Property not found");
    }

    unsigned IArgumentPropertyList::GetCount() const
    {
      return Arg ? Arg->GetCount() : 0;
    }


    IArgumentProperty::IArgumentProperty(const std::wstring &name)
      : Name(name)
    {
    }

    const std::wstring& IArgumentProperty::GetName() const
    {
      return Name;
    }

    const IArgumentProperty& IArgumentArray::operator [] (unsigned index) const
    {
      if (!std::distance(Begin(), End()))
        throw IArgumentArrayException("Empty array");
      std::wstring Index;
      {
        Common::WStringStream Io;
        Io << index;
        Index = Io.str();
      }
      for (IArgumentListCIter i = Begin() ; i != End() ; ++i)
      {
        const IArgumentProperty &Prop = dynamic_cast<const IArgumentProperty &>(*(i->Get()));
        if (Prop.GetName() == Index)
          return Prop;
      }
      throw IArgumentArrayException("Item not found");
    }

    IArgumentString::IArgumentString(const std::wstring &value)
      : String(value)
    {
    }

    const std::wstring& IArgumentString::GetString() const
    {
      return String;
    }

    IArgumentString::operator const std::wstring& () const
    {
      return String;
    }

    static const ArgumentItemType<IArgumentObject> ArgObj;
    static const ArgumentItemType<IArgumentPropertyList> ArgProp;
    static const ArgumentItemType<IArgumentString> ArgStr;
    static const ArgumentItemType<IArgumentArray> ArgArr;

    CommandPtr Command::FromNode(XmlTools::NodePtr node)
    {
      if (node->GetNodeName() != "invoke")
        throw CommandException("Bad format: \"invoke\" not found");
      CommandPtr Cmd(new Command(System::AStringToWString(node->GetPropertiesMap()["name"])));
      if (node->HasChildList())
        Cmd->LoadArguments(node->GetChildNodes());
      return Cmd;
    }

    Command::Command(const std::wstring &commandName)
      : CommandName(commandName)
    {
      //argument -> object array string
      ArgHandlers["object"] = &Command::ExtractObject;
      ArgHandlers["array"] = &Command::ExtractArray;
      ArgHandlers["string"] = &Command::ExtractString;
      //object -> property array
      ObjHandlers["property"] = &Command::ExtractProperty;
      ObjHandlers["array"] = &Command::ExtractArray;
      //property -> object array string
      PropertyHandlers["object"] = &Command::ExtractObject;
      PropertyHandlers["array"] = &Command::ExtractArray;
      PropertyHandlers["string"] = &Command::ExtractString;
      //array -> property
      ArrHandlers["property"] = &Command::ExtractProperty;
    }

    const std::wstring& Command::GetName() const
    {
      return CommandName;
    }
    
    void Command::LoadArguments(const XmlTools::NodeList &lst)
    {
      if (lst.size() != 1)
        throw CommandException("Bad arguments list");
      if (lst.front()->GetNodeName() != "arguments")
        throw CommandException("It is not arguments");
      if (!lst.front()->HasChildList())
        return;
      const XmlTools::NodeList &ArgumentList = lst.front()->GetChildNodes();
      if (ArgumentList.size() != 1)
        throw CommandException("Bad arguments list");
      ArgHandlerPool::iterator Iter = ArgHandlers.find(ArgumentList.front()->GetNodeName());
      if (Iter == ArgHandlers.end())
        throw CommandException("Unknown argument item");
      Arguments.Reset(new IArguments);
      (this->*Iter->second)(ArgumentList.front(), Arguments);
    }

    void Command::ExtractObject(XmlTools::NodePtr node, IArgumentPtr arg) const
    {
      if (!node->HasChildList())
        return;
      const XmlTools::NodeList &ObjectList = node->GetChildNodes();
      SharedPtr<IArgumentObject> Object(new IArgumentObject);
      arg->Add(Object);
      for (XmlTools::NodeList::const_iterator i = ObjectList.begin() ; i != ObjectList.end() ; ++i)
      {
        ArgHandlerPool::const_iterator Iter = ObjHandlers.find((*i)->GetNodeName());
        if (Iter == ObjHandlers.end())
          throw CommandException("Unknown object item");
        (this->*Iter->second)(*i, Object);
      }
    }

    void Command::ExtractArray(XmlTools::NodePtr node, IArgumentPtr arg) const
    {
      if (!node->HasChildList())
        return;
      const XmlTools::NodeList &ArrayList = node->GetChildNodes();
      SharedPtr<IArgumentArray> Array(new IArgumentArray);
      arg->Add(Array);
      for (XmlTools::NodeList::const_iterator i = ArrayList.begin() ; i != ArrayList.end() ; ++i)
      {
        ArgHandlerPool::const_iterator Iter = ArrHandlers.find((*i)->GetNodeName());
        if (Iter == ArrHandlers.end())
          throw CommandException("Unknown array item");
        (this->*Iter->second)(*i, Array);
      }
    }

    void Command::ExtractProperty(XmlTools::NodePtr node, IArgumentPtr arg) const
    {
      if (!node->HasChildList())
        return;
      const XmlTools::NodeList &PropList = node->GetChildNodes();
      if (PropList.size() != 1)
        throw CommandException("Bad property");
      const XmlTools::PropertyMap &PropsMap = node->GetPropertiesMap();
      SharedPtr<IArgumentProperty> Property(new IArgumentProperty(System::AStringToWString(PropsMap["id"])));
      arg->Add(Property);
      ArgHandlerPool::const_iterator Iter = PropertyHandlers.find(PropList.front()->GetNodeName());
      if (Iter == PropertyHandlers.end())
        throw CommandException("Unknown property item");
      (this->*Iter->second)(PropList.front(), Property);
    }

    void Command::ExtractString(XmlTools::NodePtr node, IArgumentPtr arg) const
    {
      if (!node->HasValue())
        return;
      SharedPtr<IArgumentString> String(new IArgumentString(node->GetValue()));
      arg->Add(String);
    }

    Array::Array()
    {
    }

    Array::Array(const Array &arr)
    {
      for (PropertyList::const_iterator i = arr.Props.begin() ; i != arr.Props.end() ; ++i)
        Props.push_back(PropertyPtr(new Property(*i->Get())));
    }

    Array& Array::operator = (const Array &arr)
    {
      if (&arr == this)
        return *this;
      for (PropertyList::const_iterator i = arr.Props.begin() ; i != arr.Props.end() ; ++i)
        Props.push_back(PropertyPtr(new Property(*i->Get())));
      return *this;
    }

    PropertyPtr Array::AddProperty()
    {
      Common::WStringStream Io;
      Io << Props.size();
      PropertyPtr Prop(new Property(Io.str()));
      Props.push_back(Prop);
      return Prop;
    }

    void Array::ToNode(XmlTools::NodePtr parent) const
    {
      XmlTools::NodePtr ArrayNode(new XmlTools::Node("array"));
      for (PropertyList::const_iterator i = Props.begin() ; i != Props.end() ; ++i)
        (*i)->ToNode(ArrayNode);
      parent->AddChildNode(ArrayNode);
    }

    Property::Property(const std::wstring &propName)
      : PropName(propName)
      , HasValue(false)
    {
    }

    Property::Property(const Property &prop)
      : PropName(prop.PropName)
      , HasValue(prop.HasValue)
    {
      if (!HasValue)
        return;
      if (prop.ArgObject.Get())
        ArgObject.Reset(new Object(*prop.ArgObject.Get()));
      else
      if (prop.ArgArray.Get())
        ArgArray.Reset(new Array(*prop.ArgArray.Get()));
      else
      if (prop.ArgString.Get())
        ArgString.Reset(new String(*prop.ArgString.Get()));
    }

    Property& Property::operator = (const Property &prop)
    {
      if (&prop == this)
        return *this;
      ArgObject.Release();
      ArgArray.Release();
      ArgString.Release();
      PropName = prop.PropName;
      HasValue = prop.HasValue;
      if (prop.ArgObject.Get())
        ArgObject.Reset(new Object(*prop.ArgObject.Get()));
      else
      if (prop.ArgArray.Get())
        ArgArray.Reset(new Array(*prop.ArgArray.Get()));
      else
      if (prop.ArgString.Get())
        ArgString.Reset(new String(*prop.ArgString.Get()));
      return *this;
    }

    ObjectPtr Property::ObjectArg()
    {
      if (!ArgObject.Get())
      {
        if (HasValue)
          throw PropertyException("Property already has value");
        HasValue = true;
        ArgObject.Reset(new Object);
      }
      return ArgObject;
    }

    ArrayPtr Property::ArrayArg()
    {
      if (!ArgArray.Get())
      {
        if (HasValue)
          throw PropertyException("Property already has value");
        HasValue = true;
        ArgArray.Reset(new Array);
      }
      return ArgArray;
    }

    StringPtr Property::StringArg(const std::wstring &s)
    {
      if (!ArgString.Get())
      {
        if (HasValue)
          throw PropertyException("Property already has value");
        HasValue = true;
        ArgString.Reset(new String(s));
      }
      return ArgString;
    }

    StringPtr Property::StringArg()
    {
      if (!ArgString.Get())
        throw PropertyException("String value not found");
      return ArgString;
    }

    void Property::ToNode(XmlTools::NodePtr parent) const
    {
      XmlTools::NodePtr PropertyNode(new XmlTools::Node("property"));
      PropertyNode->AddProperty("id", System::WStringToAString(PropName));
      if (ArgObject.Get())
        ArgObject->ToNode(PropertyNode);
      else
      if (ArgArray.Get())
        ArgArray->ToNode(PropertyNode);
      else
      if (ArgString.Get())
        ArgString->ToNode(PropertyNode);
      parent->AddChildNode(PropertyNode);
    }

    Object::Object()
    {
    }

    Object::Object(const Object &obj)
    {
      if (obj.ArgArray.Get())
        ArgArray.Reset(new Array(*obj.ArgArray.Get()));
      for (PropertyList::const_iterator i = obj.Props.begin() ; i != obj.Props.end() ; ++i)
        Props.push_back(PropertyPtr(new Property(*i->Get())));
    }

    Object& Object::operator = (const Object &obj)
    {
      if (&obj == this)
        return *this;
      ArgArray.Release();
      Props.clear();
      if (obj.ArgArray.Get())
        ArgArray.Reset(new Array(*obj.ArgArray.Get()));
      for (PropertyList::const_iterator i = obj.Props.begin() ; i != obj.Props.end() ; ++i)
        Props.push_back(PropertyPtr(new Property(*i->Get())));
      return *this;
    }

    PropertyPtr Object::AddProperty(const std::wstring &propName)
    {
      PropertyPtr Prop(new Property(propName));
      Props.push_back(Prop);
      return Prop;
    }

    ArrayPtr Object::ArrayArg()
    {
      if (!ArgArray.Get())
        ArgArray.Reset(new Array);
      return ArgArray;
    }

    void Object::ToNode(XmlTools::NodePtr parent) const
    {
      XmlTools::NodePtr ObjectNode(new XmlTools::Node("object"));
      if (ArgArray.Get())
        ArgArray->ToNode(ObjectNode);
      for (PropertyList::const_iterator i = Props.begin() ; i != Props.end() ; ++i)
        (*i)->ToNode(ObjectNode);
      parent->AddChildNode(ObjectNode);
    }

    String::String(const std::wstring &str)
      : Str(str)
    {
    }

    void String::ToNode(XmlTools::NodePtr parent) const
    {
      XmlTools::NodePtr StringNode(new XmlTools::Node("string"));
      StringNode->SetValue(Str);
      parent->AddChildNode(StringNode);
    }

    Invoker::Invoker(const std::wstring& invokerName)
      : InvokerName(invokerName)
      , HasArgument(false)
    {
    }

    Invoker::Invoker(const Invoker &invoker)
      : InvokerName(invoker.InvokerName)
      , HasArgument(invoker.HasArgument)
    {
      if (!HasArgument)
        return;
      if (invoker.ArgObject.Get())
        ArgObject.Reset(new Object(*invoker.ArgObject.Get()));
      else
      if (invoker.ArgArray.Get())
        ArgArray.Reset(new Array(*invoker.ArgArray.Get()));
      else
      if (invoker.ArgString.Get())
        ArgString.Reset(new String(*invoker.ArgString.Get()));
    }

    Invoker& Invoker::operator = (const Invoker &invoker)
    {
      if (&invoker == this)
        return *this;
      ArgObject.Release();
      ArgArray.Release();
      ArgString.Release();
      InvokerName = invoker.InvokerName;
      HasArgument = invoker.HasArgument;
      if (invoker.ArgObject.Get())
        ArgObject.Reset(new Object(*invoker.ArgObject.Get()));
      else
      if (invoker.ArgArray.Get())
        ArgArray.Reset(new Array(*invoker.ArgArray.Get()));
      else
      if (invoker.ArgString.Get())
        ArgString.Reset(new String(*invoker.ArgString.Get()));
      return *this;
    }

    ObjectPtr Invoker::ObjectArg()
    {
      if (!ArgObject.Get())
      {
        if (HasArgument)
          throw InvokerException("Argument already exists");
        HasArgument = true;
        ArgObject.Reset(new Object);
      }
      return ArgObject;
    }

    ArrayPtr Invoker::ArrayArg()
    {
      if (!ArgArray.Get())
      {
        if (HasArgument)
          throw InvokerException("Argument already exists");
        HasArgument = true;
        ArgArray.Reset(new Array);
      }
      return ArgArray;
    }

    StringPtr Invoker::StringArg(const std::wstring &s)
    {
      if (!ArgString.Get())
      {
        if (HasArgument)
          throw InvokerException("Argument already exists");
        HasArgument = true;
        ArgString.Reset(new String(s));
      }
      return ArgString;
    }

    StringPtr Invoker::StringArg()
    {
      if (!ArgString.Get())
        throw InvokerException("String argument not found");
      return ArgString;
    }

    XmlTools::NodePtr Invoker::ToNode() const
    {
      XmlTools::NodePtr InvokerNode(new XmlTools::Node("invoke"));
      InvokerNode->AddProperty("name", System::WStringToAString(InvokerName));
      InvokerNode->AddProperty("returntype", "xml");
      XmlTools::NodePtr ArgumentsNode(new XmlTools::Node("arguments"));
      InvokerNode->AddChildNode(ArgumentsNode);
      if (ArgObject.Get())
        ArgObject->ToNode(ArgumentsNode);
      else
      if (ArgArray.Get())
        ArgArray->ToNode(ArgumentsNode);
      else
      if (ArgString.Get())
        ArgString->ToNode(ArgumentsNode);
      return InvokerNode;
    }

  }
}
