//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include "../Pointers.h"
#include "../Exceptions.h"
#include "../SystemUtils.h"
#include "../Typedefs.h"
#include "XmlTools.h"

#include <list>
#include <string>
#include <map>


namespace Common
{
  namespace Commands
  {

    class IArgument;
    class IArgumentObject;
    class IArgumentProperty;
    class IArgumentArray;
    class IArgumentString;
    class IArgumentPropertyList;
    class Command;

    typedef SharedPtr<IArgument> IArgumentPtr;
    typedef std::list<IArgumentPtr> IArgumentList;
    typedef SharedPtr<IArgumentList> IArgumentListPtr;
    typedef IArgumentList::iterator IArgumentListIter;
    typedef IArgumentList::const_iterator IArgumentListCIter;
    typedef SharedPtr<Command> CommandPtr;

    template <typename T>
    struct ArgumentItemType
    {
      typedef T Type;
    };

    DECLARE_LOGIC_EXCEPTION(IArgument)

    class IArgument
    {
    public:
      virtual ~IArgument();
      void Add(IArgumentPtr arg);
      IArgumentListIter Begin();
      IArgumentListCIter Begin() const;
      IArgumentListIter End();
      IArgumentListCIter End() const;
      unsigned GetCount() const;
    private:
      IArgumentListPtr Arguments;
    };

    class IArguments : public IArgument
    {
    };

    DECLARE_LOGIC_EXCEPTION(IArgumentPropertyList)

    class IArgumentPropertyList
    {
    public:
      IArgumentPropertyList(const IArgument *arg = 0);
      const IArgumentProperty& operator [] (const std::wstring &name) const;
      const IArgumentProperty& operator [] (unsigned index) const;
      IArgumentPropertyList(const IArgumentPropertyList &lst);
      IArgumentPropertyList& operator = (const IArgumentPropertyList &lst);
      unsigned GetCount() const;
    private:
      IArgumentPtr CopyArg;
      const IArgument *Arg;
    };

    DECLARE_LOGIC_EXCEPTION(IArgumentObject)

    class IArgumentObject : public IArgument
    {
    public:
      template <typename T>
      const typename T::Type& operator [] (const T &) const
      {
        if (!std::distance(Begin(), End()))
          throw IArgumentObjectException("No object items");
        typename T::Type *RetValue = 0;
        Get(&RetValue);
        return *RetValue;
      }
    private:
      mutable SharedPtr<IArgumentPropertyList> PropList;
      void Get(IArgumentPropertyList **result) const
      {
        PropList.Reset(new IArgumentPropertyList(this));
        *result = PropList.Get();
      }
    };

    DECLARE_LOGIC_EXCEPTION(IArgumentProperty)

    class IArgumentProperty : public IArgument
    {
    public:
      IArgumentProperty(const std::wstring &name);
      const std::wstring& GetName() const;
      template <typename T>
      const typename T::Type& operator [] (const T &) const
      {
        if (std::distance(Begin(), End()) != 1)
          throw IArgumentPropertyException("Bad property");
        return dynamic_cast<const typename T::Type&>(*(Begin()->Get()));
      }
    private:
      std::wstring Name;
    };

    DECLARE_LOGIC_EXCEPTION(IArgumentArray)

    class IArgumentArray : public IArgument
    {
    public:
      const IArgumentProperty& operator [] (unsigned index) const;
    };

    class IArgumentString : public IArgument
    {
    public:
      IArgumentString(const std::wstring &value);
      const std::wstring& GetString() const;
      operator const std::wstring& () const;
    private:
      std::wstring String;
    };

    extern const ArgumentItemType<IArgumentObject> ArgObj;
    extern const ArgumentItemType<IArgumentPropertyList> ArgProp;
    extern const ArgumentItemType<IArgumentString> ArgStr;
    extern const ArgumentItemType<IArgumentArray> ArgArr;

    DECLARE_LOGIC_EXCEPTION(Command)

    class Command
    {
    public:
      static CommandPtr FromNode(XmlTools::NodePtr node);
      const std::wstring& GetName() const;
      template <typename T>
      const typename T::Type& operator [] (const T &) const
      {
        if (!Arguments.Get() || std::distance(Arguments->Begin(), Arguments->End()) != 1)
          throw CommandException("No arguments");
        return dynamic_cast<const typename T::Type&>(*Arguments->Begin()->Get());
      }
    private:
      typedef void (Command::*PArgHandler)(XmlTools::NodePtr, IArgumentPtr) const;
      typedef std::map<std::string, PArgHandler> ArgHandlerPool;
      ArgHandlerPool ArgHandlers;
      ArgHandlerPool ObjHandlers;
      ArgHandlerPool PropertyHandlers;
      ArgHandlerPool ArrHandlers;
      std::wstring CommandName;
      SharedPtr<IArguments> Arguments;
      Command(const std::wstring &commandName);
      void LoadArguments(const XmlTools::NodeList &lst);
      void ExtractObject(XmlTools::NodePtr node, IArgumentPtr arg) const;
      void ExtractArray(XmlTools::NodePtr node, IArgumentPtr arg) const;
      void ExtractProperty(XmlTools::NodePtr node, IArgumentPtr arg) const;
      void ExtractString(XmlTools::NodePtr node, IArgumentPtr arg) const;
    };

    class Object;
    class Array;
    class Property;
    class String;
    class Invoker;

    typedef SharedPtr<Object> ObjectPtr;
    typedef SharedPtr<Array> ArrayPtr;
    typedef SharedPtr<Property> PropertyPtr;
    typedef SharedPtr<String> StringPtr;
    typedef SharedPtr<Invoker> InvokerPtr;

    struct INodeCreator
    {
      virtual ~INodeCreator() {}
      virtual void ToNode(XmlTools::NodePtr parent) const = 0;
    };

    class Array
      : public INodeCreator
    {
    public:
      Array();
      Array(const Array &arr);
      Array& operator = (const Array &arr);
      PropertyPtr AddProperty();
      virtual void ToNode(XmlTools::NodePtr parent) const;
    private:
      typedef std::list<PropertyPtr> PropertyList;
      PropertyList Props;
    };

    DECLARE_LOGIC_EXCEPTION(Property)

    class Property
      : public INodeCreator
    {
    public:
      Property(const std::wstring &propName);
      Property(const Property &prop);
      Property& operator = (const Property &prop);
      ObjectPtr ObjectArg();
      ArrayPtr ArrayArg();
      StringPtr StringArg(const std::wstring &s);
      StringPtr StringArg();
      virtual void ToNode(XmlTools::NodePtr parent) const;
    private:
      std::wstring PropName;
      bool HasValue;
      ObjectPtr ArgObject;
      ArrayPtr ArgArray;
      StringPtr ArgString;
    };

    class Object
      : public INodeCreator
    {
    public:
      Object();
      Object(const Object &obj);
      Object& operator = (const Object &obj);
      PropertyPtr AddProperty(const std::wstring &propName);
      ArrayPtr ArrayArg();
      virtual void ToNode(XmlTools::NodePtr parent) const;
    private:
      ArrayPtr ArgArray;
      typedef std::list<PropertyPtr> PropertyList;
      PropertyList Props;
    };

    class String
      : public INodeCreator
    {
    public:
      String(const std::wstring &str);
      virtual void ToNode(XmlTools::NodePtr parent) const;
    private:
      std::wstring Str;
    };

    DECLARE_LOGIC_EXCEPTION(Invoker)

    class Invoker
    {
    public:
      Invoker(const std::wstring& invokerName);
      Invoker(const Invoker &invoker);
      Invoker& operator = (const Invoker &invoker);
      ObjectPtr ObjectArg();
      ArrayPtr ArrayArg();
      StringPtr StringArg(const std::wstring &s);
      StringPtr StringArg();
      XmlTools::NodePtr ToNode() const;
    private:
      std::wstring InvokerName;
      bool HasArgument;
      ObjectPtr ArgObject;
      ArrayPtr ArgArray;
      StringPtr ArgString;
    };

  }
}


#endif  // !__COMMANDS_H__
