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
    typedef std::list<NodePtr> NodeList;
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
          Properties = new StringMap;
        (*Properties.Get())[name] = value;
      }
      const StringMap& GetPropertiesMap() const
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
        if (ChildNodes.Get())
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
      SharedPtr<StringMap> Properties;
      NodeListPtr ChildNodes;
    };

    NodePtr XmlToNode(const std::string &xml);
    std::string NodeToUTF8(const Node &node, bool addHeader = true);

  }
}

#endif  // !__XMLTOOLS_H__
