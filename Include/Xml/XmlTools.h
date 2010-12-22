//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __XMLTOOLS_H__
#define __XMLTOOLS_H__

#ifdef _MSC_VER
  #pragma warning (disable : 4244)
#endif

#include "./TinyXml/tinyxml.h"
#include "../Utf8/utf8.h"
#include "../Pointers.h"
#include "../Typedefs.h"
#include "../Exceptions.h"


#include <string>
#include <map>
#include <list>

namespace Common
{
  namespace XmlTools
  {
    DECLARE_LOGIC_EXCEPTION(Node)

    class Node;

    typedef SharedPtr<Node> NodePtr;

    class Tag
    {
    public:
      Tag(const std::string &name)
        : Name(name)
      {
      }
      const std::string& GetName() const
      {
        return Name;
      }
    private:
      std::string Name;
    };

    class Attribute
    {
    public:
      Attribute(const std::string &name, const std::string &value = "")
        : Name(name)
        , Value(value)
      {
      }
      const std::string& GetName() const
      {
        return Name;
      }
      const std::string& GetValue() const
      {
        return Value;
      }
    private:
      std::string Name;
      std::string Value;
    };


    DECLARE_LOGIC_EXCEPTION(NodeList)

    class NodeList
      : private std::list<NodePtr>
    {
    public:
      using std::list<NodePtr>::const_iterator;
      using std::list<NodePtr>::push_back;
      using std::list<NodePtr>::begin;
      using std::list<NodePtr>::end;
      using std::list<NodePtr>::front;
      using std::list<NodePtr>::size;

      NodeList()
      {
      }
      NodeList(const NodePtr node)
      {
        push_back(node);
      }
      inline NodeList operator [] (const Tag &tag) const;
      inline NodeList operator [] (const Attribute &attribute) const;
      inline const NodePtr operator [] (unsigned index) const;
    };

    DECLARE_LOGIC_EXCEPTION(PropertyMap)

    class PropertyMap
      : private StringMap
    {
    public:
      using StringMap::const_iterator;
      using StringMap::begin;
      using StringMap::end;
      using StringMap::find;

      void Add(const std::string &name, const std::string &value)
      {
        StringMap &Map = *this;
        Map[name] = value;
      }

      const std::string& operator [] (const std::string name) const
      {
        const_iterator Iter = find(name);
        if (Iter == end())
          throw PropertyMapException("Property not found");
        return Iter->second;
      }
    };

    typedef SharedPtr<NodeList> NodeListPtr;

    class Node
    {
    public:
      Node(const std::string &nodeName)
        : NodeName(nodeName)
      {
      }
      const std::string& GetNodeName() const
      {
        return NodeName;
      }
      void SetValue(const std::wstring &value)
      {
        if (ChildNodes.Get())
          throw NodeException("Can't set value. Node has child node");
        if (!Value.Get())
          Value = new std::wstring;
        (*Value.Get()) = value;
      }
      const std::wstring& GetValue() const
      {
        if (!Value.Get())
          throw NodeException("No value");
        return *Value.Get();
      }
      bool HasValue() const
      {
        return !!Value.Get();
      }
      void AddProperty(const std::string &name, const std::string &value)
      {
        if (!Properties.Get())
          Properties = new PropertyMap;
        Properties->Add(name, value);
      }
      const PropertyMap& GetPropertiesMap() const
      {
        if (!Properties.Get())
          throw NodeException("No properties");
        return *Properties.Get();
      }
      bool HasProperty() const
      {
        return !!Properties.Get();
      }
      void AddChildNode(NodePtr childNode)
      {
        if (!ChildNodes.Get())
          ChildNodes = new NodeList;
        ChildNodes->push_back(childNode);
      }
      const NodeList& GetChildNodes() const
      {
        if (!ChildNodes.Get())
          throw NodeException("No child");
        return *ChildNodes.Get();
      }
      bool HasChildList() const
      {
        return !!ChildNodes.Get();
      }
      const NodeListPtr GetChildListByName(const std::string &nodeName) const
      {
        if (!ChildNodes.Get())
          throw NodeException("No child");
        NodeListPtr ResList(new NodeList);
        for (NodeList::const_iterator i = ChildNodes->begin() ; i != ChildNodes->end() ; ++i)
        {
          if ((*i)->GetNodeName() == nodeName)
            ResList->push_back(*i);
        }
        if (!ResList->size())
          throw NodeException("Empty list by name");
        return ResList;
      }
      const NodeListPtr GetChildListByProp(const std::string &propName, const std::string &propValue = "") const
      {
        if (!Properties.Get())
          throw NodeException("No properties");
        NodeListPtr ResList(new NodeList);
        for (NodeList::const_iterator i = ChildNodes->begin() ; i != ChildNodes->end() ; ++i)
        {
          if ((*i)->HasProperty())
          {
            StringMap::const_iterator Iter = Properties->find(propName);
            if (Iter != Properties->end())
            {
              if (!propValue.empty() && Iter->second != propValue)
                continue;
              ResList->push_back(*i);
            }
          }
        }
        if (!ResList->size())
          throw NodeException("Empty list by property");
        return ResList;
      }
    private:
      std::string NodeName;
      SharedPtr<std::wstring> Value;
      SharedPtr<PropertyMap> Properties;
      NodeListPtr ChildNodes;
    };

    inline NodeList NodeList::operator [] (const Tag &tag) const
    {
      NodeList Res;
      for (const_iterator i = begin() ; i != end() ; ++i)
      {
        if ((*i)->GetNodeName() == tag.GetName())
          Res.push_back(*i);
      }
      if (Res.empty())
        throw NodeListException("Empty list");
      return Res;
    }

    inline NodeList NodeList::operator [] (const Attribute &attribute) const
    {
      NodeList Res;
      for (const_iterator i = begin() ; i != end() ; ++i)
      {
        if ((*i)->HasProperty())
        {
          const PropertyMap &Props = (*i)->GetPropertiesMap();
          PropertyMap::const_iterator Iter = Props.find(attribute.GetName());
          if (Iter == Props.end())
            continue;
          if (!attribute.GetValue().empty() && Iter->second != attribute.GetValue())
            continue;
          Res.push_back(*i);
        }
      }
      if (Res.empty())
        throw NodeListException("Empty list");
      return Res;
    }

    inline const NodePtr NodeList::operator [] (unsigned index) const
    {
      unsigned Index = 0;
      for (const_iterator i = begin() ; i != end() ; ++i)
      {
        if (Index++ == index)
          return *i;
      }
      throw NodeListException("Bad index");
    }

    NodePtr XmlToNode(const std::string &xml);
    std::string NodeToUTF8(const Node &node, bool addHeader = true, bool useFormat = true);

  }
}

#endif  // !__XMLTOOLS_H__
