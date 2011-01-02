//============================================================================
// Date        : 02.01.2011
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __XMLRPC_H__
#define __XMLRPC_H__

#include <map>
#include <string>
#include <vector>
#include <typeinfo>
#include <sstream>

#include "./rapidxml/rapidxml.hpp"

#include "Exceptions.h"
#include "SharedPtr.h"
#include "NoCopyable.h"


namespace XmlRpc
{

  namespace Cmds
  {

    typedef rapidxml::xml_node<> Node;
    typedef rapidxml::xml_attribute<> Attribute;

    class Manifest;
    class Parameters;
    class Parameter;

    typedef Parameters Object;
    typedef std::vector<Common::SharedPtr<Parameter> > Array;

    DECLARE_RUNTIME_EXCEPTION(Command)

    class Command
      : private Common::NoCopyable
    {
    public:
      inline Command(const std::string &xml);
      inline std::string GetName() const;
      inline std::string GetService() const;
      inline const Manifest& GetManifest() const;
      inline const Parameters& GetParams() const;

    private:
      rapidxml::xml_document<> Doc;
      Node *Cmd;
      mutable Common::SharedPtr<Manifest> ManifestObj;
      mutable Common::SharedPtr<Parameters> ParamsObj;
    };


    DECLARE_RUNTIME_EXCEPTION(Manifest)

    class Manifest
      : private Common::NoCopyable
    {
    public:
      std::string GetVersion() const;
    private:
      friend const Manifest& Command::GetManifest() const;
      const Node *Item;
      Manifest(const Node *item);
    };


    DECLARE_RUNTIME_EXCEPTION(Parameters)

    class Parameters
      : private Common::NoCopyable
    {
    public:
      inline Parameter& operator [] (const char *prmName) const;
      inline Parameter& operator [] (const std::string &prmName) const;
    private:
      friend const Parameters& Command::GetParams() const;
      friend class Parameter;
      const Node *Item;
      typedef Common::SharedPtr<Parameter> ParameterPtr;
      typedef std::map<std::string, ParameterPtr> ParamsPool;
      mutable ParamsPool Params;
      inline Parameters(const Node *item);
    };


    DECLARE_RUNTIME_EXCEPTION(Parameter)

    class Parameter
      : private Common::NoCopyable
    {
    public:
      template <typename T>
      inline T GetValue() const;
      inline const Object& GetObject() const;
      inline const Array& GetArray() const;
      inline const std::string& GetType() const;
    private:
      friend Parameter& Parameters::operator [] (const char *prmName) const;
      const Node *Item;
      mutable std::string Type;
      mutable Common::SharedPtr<Object> ObjectVal;
      mutable Common::SharedPtr<Array> ArrayVal;
      inline Parameter(const Node *item);
    };


    namespace Internal
    {
      bool EqualStrings(const char *s1, const char *s2)
      {
        while ((*s1 && *s2) && (*s1 == *s2) && (s1++ && s2++));
        return !(*s1 || *s2);
      }
    }

    inline Command::Command(const std::string &xml)
      : Cmd(0)
    {
      Doc.parse<rapidxml::parse_default>(const_cast<char *>(xml.c_str()));
      Cmd = Doc.first_node("Command");
      if (!Cmd)
        throw CommandException("Command tag not found");
    }

    inline std::string Command::GetName() const
    {
      for (Attribute *i = Cmd->first_attribute("name") ; i ; i = i->next_attribute())
      {
        const char *AttrName = i->name();
        if (AttrName && Internal::EqualStrings("name", AttrName))
        {
          const char *Name = i->value();
          if (!Name)
            throw CommandException("Error get command name");
          return Name;
        }
      }
      throw CommandException("Error get command name");
    }

    inline std::string Command::GetService() const
    {
      for (Attribute *i = Cmd->first_attribute("service") ; i ; i = i->next_attribute())
      {
        const char *AttrName = i->name();
        if (AttrName && Internal::EqualStrings("service", AttrName))
        {
          const char *Service = i->value();
          if (!Service)
            throw CommandException("Error get service");
          return Service;
        }
      }
      throw CommandException("Error get command name");
    }

    inline const Manifest& Command::GetManifest() const
    {
      if (!ManifestObj.Get())
        ManifestObj.Reset(new Manifest(Cmd->first_node("Manifest")));
      return *ManifestObj.Get();
    }

    inline const Parameters& Command::GetParams() const
    {
      if (!ParamsObj.Get())
        ParamsObj.Reset(new Parameters(Cmd->first_node("Params")));
      return *ParamsObj.Get();
    }

    inline Manifest::Manifest(const Node *item)
      : Item(item)
    {
      if (!Item)
        throw ManifestException("Empty manifest");
    }

    inline std::string Manifest::GetVersion() const
    {
      Node *VerItem = Item->first_node("Version");
      if (!VerItem)
        throw ManifestException("Error get version");
      const char *Ver = VerItem->value();
      if (!Ver)
        throw ManifestException("Error get version");
      return Ver;
    }

    inline Parameters::Parameters(const Node *item)
      : Item(item)
    {
      if (!Item)
        throw ManifestException("Empty parameters");
    }

    inline Parameter& Parameters::operator [] (const char *prmName) const
    {
      ParamsPool::const_iterator Iter = Params.find(prmName);
      if (Iter != Params.end())
        return *Iter->second.Get();
      for (Node *i = Item->first_node("Prm") ; i ; i = i->next_sibling("Prm"))
      {
        Attribute *NameAttr = i->first_attribute("name");
        if (!NameAttr)
          throw ParametersException("Bad parameter");
        if (!NameAttr->name())
          throw ParametersException("Bad parameter");
        const char *Name = NameAttr->value();
        if (!Name)
          throw ParametersException("Bad parameter");
        if (!Internal::EqualStrings(Name, prmName))
          continue;
        ParameterPtr NewParam(new Parameter(i));
        Params[Name] = NewParam;
        return *NewParam.Get();
      }
      throw ParametersException("Parameter not found");
    }

    inline Parameter& Parameters::operator [] (const std::string &prmName) const
    {
      return (*this)[prmName.c_str()];
    }


    namespace Internal
    {
    
      std::string ExtractValue(const Node *item)
      {
        if (item->type() == rapidxml::node_element)
        {
          item = item->first_node();
          if (!item)
            throw std::runtime_error("Error get value");
        }
        const char *RawVal = item->value();
        if (!RawVal)
          throw std::runtime_error("Error get value");
        return RawVal;
      }

      void CheckType(const std::string &type1, const char *type2)
      {
        if (type1 != type2)
          throw std::bad_cast();
      }

      template <typename T>
      struct StrToValue;

      template <>
      struct StrToValue<unsigned char>
      {
        static unsigned char Get(const std::string &type, const Node *item)
        {
          CheckType(type, "unsigned char");
          return static_cast<unsigned char>(::atoi(ExtractValue(item).c_str()));
        }
      };

      template <>
      struct StrToValue<char>
      {
        static char Get(const std::string &type, const Node *item)
        {
          CheckType(type, "char");
          return static_cast<char>(::atoi(ExtractValue(item).c_str()));
        }
      };

      template <>
      struct StrToValue<unsigned short>
      {
        static unsigned short Get(const std::string &type, const Node *item)
        {
          CheckType(type, "unsigned short");
          return static_cast<unsigned short>(::atoi(ExtractValue(item).c_str()));
        }
      };

      template <>
      struct StrToValue<short>
      {
        static short Get(const std::string &type, const Node *item)
        {
          CheckType(type, "short");
          return static_cast<short>(::atoi(ExtractValue(item).c_str()));
        }
      };

      template <>
      struct StrToValue<unsigned int>
      {
        static unsigned int Get(const std::string &type, const Node *item)
        {
          CheckType(type, "unsigned int");
          return static_cast<unsigned int>(::atoi(ExtractValue(item).c_str()));
        }
      };

      template <>
      struct StrToValue<int>
      {
        static int Get(const std::string &type, const Node *item)
        {
          CheckType(type, "int");
          return static_cast<int>(::atoi(ExtractValue(item).c_str()));
        }
      };

      template <>
      struct StrToValue<unsigned long>
      {
        static unsigned long Get(const std::string &type, const Node *item)
        {
          CheckType(type, "unsigned long");
          return static_cast<unsigned long>(::atol(ExtractValue(item).c_str()));
        }
      };

      template <>
      struct StrToValue<long>
      {
        static long Get(const std::string &type, const Node *item)
        {
          CheckType(type, "long");
          return static_cast<long>(::atol(ExtractValue(item).c_str()));
        }
      };

      template <>
      struct StrToValue<double>
      {
        static double Get(const std::string &type, const Node *item)
        {
          CheckType(type, "double");
          return ::atof(ExtractValue(item).c_str());
        }
      };

      template <>
      struct StrToValue<float>
      {
        static float Get(const std::string &type, const Node *item)
        {
          CheckType(type, "float");
          return static_cast<float>(::atof(ExtractValue(item).c_str()));
        }
      };

      template <>
      struct StrToValue<std::string>
      {
        static std::string Get(const std::string &type, const Node *item)
        {
          CheckType(type, "string");
          return ExtractValue(item);
        }
      };

      template <>
      struct StrToValue<const char *>
      {
        static std::string Get(const std::string &type, const Node *item)
        {
          CheckType(type, "string");
          return ExtractValue(item);
        }
      };

      template <>
      struct StrToValue<bool>
      {
        static bool Get(const std::string &type, const Node *item)
        {
          CheckType(type, "bool");
          std::string Value = ExtractValue(item);
          if (Value != "true" && Value != "false")
            throw std::runtime_error("Bad bool value");
          return Value == "true";
        }
      };

    }

    inline Parameter::Parameter(const Node *item)
      : Item(item)
    {
      if (!Item)
        throw ParameterException("Empty parameter");
    }

    template <typename T>
    inline T Parameter::GetValue() const
    {
      return Internal::StrToValue<T>::Get(GetType(), Item);
    }

    inline const Object& Parameter::GetObject() const
    {
      if (!ObjectVal.Get())
      {
        if (GetType() != "object")
          throw std::bad_cast();
        ObjectVal.Reset(new Parameters(Item));
      }
      return *ObjectVal.Get();
    }

    inline const Array& Parameter::GetArray() const
    {
      if (!ArrayVal.Get())
      {
        typedef Common::SharedPtr<Parameter> ParameterPtr;
        typedef std::map<unsigned, ParameterPtr> PrmPool;
        PrmPool Prms;
        for (const Node *i = Item->first_node("Item") ; i ; i = i->next_sibling("Item"))
        {
          Attribute *AttrId = i->first_attribute("id");
          if (!AttrId)
            throw ParameterException("Bad array item");
          const char *RawValId = AttrId->value();
          if (!RawValId || !*RawValId)
            throw ParameterException("Bad array item");
          Prms[static_cast<unsigned>(atoi(RawValId))] = ParameterPtr(new Parameter(i->first_node("Prm")));
        }
        ArrayVal.Reset(new Array);
        for (PrmPool::const_iterator i = Prms.begin() ; i != Prms.end() ; ++i)
          ArrayVal->push_back(i->second);
      }
      return *ArrayVal.Get();
    }

    inline const std::string& Parameter::GetType() const
    {
      if (Type.empty())
      {
        Attribute *AttrType = Item->first_attribute("type");
        if (!AttrType)
          ParameterException("Empty parameter type");
        const char *AttrVal = AttrType->value();
        if (!AttrVal || !*AttrVal)
          throw ParameterException("Empty parameter type");
        Type = AttrVal;
      }
      return Type;
    }

  }


  namespace Inv
  {

    typedef std::pair<std::string, std::string> StringPair;

    class Object;
    class Array;


    class Array
      : private Common::NoCopyable
    {
    public:
      template <typename T>
      inline void AddValue(const T &val);
      inline void AddValue(const char *val);
      inline Object& AddObject();
      inline Array& AddArray();
      inline void ToXml(std::ostream &os) const;

    private:
      typedef std::vector<StringPair> ValuePool;
      ValuePool Values;
      typedef Common::SharedPtr<Object> ObjectPtr;
      typedef std::vector<ObjectPtr> ObjectPool;
      ObjectPool Objects;
      typedef Common::SharedPtr<Array> ArrayPtr;
      typedef std::vector<ArrayPtr> ArrayPool;
      ArrayPool Arrays;
    };

    class Object
      : private Common::NoCopyable
    {
    public:
      template <typename T>
      inline void SetValue(const std::string &prmName, const T &val);
      inline void SetValue(const std::string &prmName, const char *val);
      inline Object& GetObject(const std::string &objName);
      inline Array& GetArray(const std::string &arrName);
      inline void ToXml(std::ostream &os) const;

    private:
      typedef std::map<std::string, StringPair> ValuePool;
      ValuePool Values;
      typedef Common::SharedPtr<Object> ObjectPtr;
      typedef std::map<std::string, ObjectPtr> ObjectPool;
      ObjectPool Objects;
      typedef Common::SharedPtr<Array> ArrayPtr;
      typedef std::map<std::string, ArrayPtr> ArrayPool;
      ArrayPool Arrays;
    };

    class Invoker
      : public Object
    {
    public:
      inline Invoker(const std::string &service, const std::string &name);
      inline const std::string ToXml() const;

    private:
      static const char Version[];
      std::string Service;
      std::string Name;
    };


    namespace Internal
    {
      template <typename T>
      std::string ToString(const T &val)
      {
        std::stringstream Io;
        Io << val;
        return Io.str();
      }

      template <typename T>
      struct ValueToPair;

      template <>
      struct ValueToPair<unsigned char>
      {
        static StringPair Set(unsigned char val)
        {
          return StringPair(ToString(static_cast<unsigned long>(val)), "unsigned char");
        }
      };

      template <>
      struct ValueToPair<char>
      {
        static StringPair Set(char val)
        {
          return StringPair(ToString(static_cast<long>(val)), "char");
        }
      };

      template <>
      struct ValueToPair<unsigned short>
      {
        static StringPair Set(unsigned short val)
        {
          return StringPair(ToString(static_cast<unsigned long>(val)), "unsigned short");
        }
      };

      template <>
      struct ValueToPair<short>
      {
        static StringPair Set(short val)
        {
          return StringPair(ToString(static_cast<long>(val)), "short");
        }
      };

      template <>
      struct ValueToPair<unsigned int>
      {
        static StringPair Set(unsigned int val)
        {
          return StringPair(ToString(static_cast<unsigned long>(val)), "unsigned int");
        }
      };

      template <>
      struct ValueToPair<int>
      {
        static StringPair Set(int val)
        {
          return StringPair(ToString(static_cast<long>(val)), "int");
        }
      };

      template <>
      struct ValueToPair<unsigned long>
      {
        static StringPair Set(unsigned long val)
        {
          return StringPair(ToString(static_cast<unsigned long>(val)), "unsigned long");
        }
      };

      template <>
      struct ValueToPair<long>
      {
        static StringPair Set(long val)
        {
          return StringPair(ToString(static_cast<long>(val)), "long");
        }
      };

      template <>
      struct ValueToPair<double>
      {
        static StringPair Set(double val)
        {
          return StringPair(ToString(val), "double");
        }
      };

      template <>
      struct ValueToPair<float>
      {
        static StringPair Set(float val)
        {
          return StringPair(ToString(val), "float");
        }
      };

      template <>
      struct ValueToPair<bool>
      {
        static StringPair Set(bool val)
        {
          return StringPair(val ? "true" : "false", "bool");
        }
      };

      template <>
      struct ValueToPair<std::string>
      {
        static StringPair Set(const std::string &val)
        {
          return StringPair(val, "string");
        }
      };

      template <>
      struct ValueToPair<const char *>
      {
        static StringPair Set(const char *val)
        {
          return StringPair(val, "string");
        }
      };

    }

    const char Invoker::Version[] = "1.0.0.0";

    inline Invoker::Invoker(const std::string &service, const std::string &name)
      : Service(service)
      , Name(name)
    {
    }

    inline const std::string Invoker::ToXml() const
    {
      std::stringstream Xml;
      Xml
        << "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
        << "<Command name ='" << Name << "' service ='" << Service << "'>"
        << "<Manifest>"
        << "<Version>" << Version << "</Version>"
        << "</Manifest>"
        << "<Params>";

      Object::ToXml(Xml);

      Xml
        << "</Params>"
        << "</Command>";
      return Xml.str();
    }

    template <typename T>
    inline void Array::AddValue(const T &val)
    {
      Values.push_back(Internal::ValueToPair<T>::Set(val));
    }

    inline void Array::AddValue(const char *val)
    {
      Values.push_back(Internal::ValueToPair<const char *>::Set(val));
    }

    inline Object& Array::AddObject()
    {
      ObjectPtr Obj(new Object);
      Objects.push_back(Obj);
      return *Obj.Get();
    }

    inline Array& Array::AddArray()
    {
      ArrayPtr Arr(new Array);
      Arrays.push_back(Arr);
      return *Arr.Get();
    }

    inline void Array::ToXml(std::ostream &os) const
    {
      if (!Values.empty())
      {
        unsigned Index = 0;
        for (ValuePool::const_iterator i = Values.begin() ; i != Values.end() ; ++i)
        {
          os
            << "<Item id ='" << Index++ << "'>"
            << "<Prm type ='" << i->second << "'>"
            << i->first
            << "</Prm>"
            << "</Item>";
        }
      }

      if (!Objects.empty())
      {
        for (ObjectPool::const_iterator i = Objects.begin() ; i != Objects.end() ; ++i)
        {
          os << "<Prm type ='object'>";
          (*i)->ToXml(os);
          os << "</Prm>";
        }
      }

      if (!Arrays.empty())
      {
        for (ArrayPool::const_iterator i = Arrays.begin() ; i != Arrays.end() ; ++i)
        {
          os << "<Prm type ='array'>";
          (*i)->ToXml(os);
          os << "</Prm>";
        }
      }
    }

    template <typename T>
    inline void Object::SetValue(const std::string &prmName, const T &val)
    {
      ValuePool::iterator Iter = Values.find(prmName);
      if (Iter == Values.end())
        Values[prmName] = Internal::ValueToPair<T>::Set(val);
      else
        Iter->second = Internal::ValueToPair<T>::Set(val);
    }

    inline void Object::SetValue(const std::string &prmName, const char *val)
    {
      ValuePool::iterator Iter = Values.find(prmName);
      if (Iter == Values.end())
        Values[prmName] = Internal::ValueToPair<const char *>::Set(val);
      else
        Iter->second = Internal::ValueToPair<const char *>::Set(val);
    }

    inline Object& Object::GetObject(const std::string &objName)
    {
      ObjectPool::iterator Iter = Objects.find(objName);
      if (Iter == Objects.end())
      {
        ObjectPtr Obj(new Object);
        Objects[objName] = Obj;
        return *Obj.Get();
      }
      return *Iter->second.Get();
    }

    inline Array& Object::GetArray(const std::string &arrName)
    {
      ArrayPool::iterator Iter = Arrays.find(arrName);
      if (Iter == Arrays.end())
      {
        ArrayPtr Arr(new Array);
        Arrays[arrName] = Arr;
        return *Arr.Get();
      }
      return *Iter->second.Get();
    }

    inline void Object::ToXml(std::ostream &os) const
    {
      if (!Values.empty())
      {
        for (ValuePool::const_iterator i = Values.begin() ; i != Values.end() ; ++i)
        {
          os
            << "<Prm " << "name ='" << i->first << "' type ='" << i->second.second << "'>"
            << i->second.first
            << "</Prm>";
        }
      }

      if (!Objects.empty())
      {
        for (ObjectPool::const_iterator i = Objects.begin() ; i != Objects.end() ; ++i)
        {
          os << "<Prm " << "name ='" << i->first << "' type ='object'>";
          i->second->ToXml(os);
          os << "</Prm>";
        }
      }

      if (!Arrays.empty())
      {
        for (ArrayPool::const_iterator i = Arrays.begin() ; i != Arrays.end() ; ++i)
        {
          os << "<Prm " << "name ='" << i->first << "' type ='array'>";
          i->second->ToXml(os);
          os << "</Prm>";
        }
      }
    }

  }

}

#endif  // !__XMLRPC_H__
