#include "Xml/XmlTools.h"

namespace Common
{
  namespace XmlTools
  {

    NodePtr XmlToNode(const std::string &xml)
    {
      struct
      {
        void operator () (const TiXmlNode *xmlNode, NodePtr node)
        {
          const TiXmlElement *Element = xmlNode->ToElement();
          if (Element)
          {
            for (const TiXmlAttribute *i = Element->FirstAttribute() ; i ; i = i->Next())
            {
              std::string AttrName = i->Name();
              if (AttrName.empty())
                throw NodeException("Empty attribute name");
              std::string AttrValue = i->ValueStr();
              if (AttrValue.empty())
                throw NodeException("Empty attribute value");
              node->AddProperty(AttrName, AttrValue);
            }
          }
          if (xmlNode->Type() == TiXmlNode::TEXT)
          {
            std::string Value = xmlNode->ValueStr();
            if (!Value.empty())
            {
              if (utf8::find_invalid(Value.begin(), Value.end()) != Value.end())
                throw NodeException("Invalide value");
            }
            std::wstring WValue;
            utf8::utf8to16(Value.begin(), Value.end(), std::back_inserter(WValue));
            node->SetValue(WValue);
            return;
          }
          for (const TiXmlNode *i = xmlNode->FirstChild() ; i ; i = i->NextSibling())
          {
            if (i->Type() == TiXmlNode::TEXT)
            {
              (*this)(i, node);
              continue;
            }
            std::string Name = i->ValueStr();
            if (Name.empty())
              throw NodeException("Empty node name");
            NodePtr NewNode(new Node(Name));
            node->AddChildNode(NewNode);
            (*this)(i, NewNode);
          }
        }
      } ParseXml;
      TiXmlDocument Doc;
      Doc.Parse(xml.c_str());
      const TiXmlNode *Root = Doc.RootElement();
      if (!Root)
        throw NodeException("Can't get root element");
      std::string RootName = Root->ValueStr();
      NodePtr Ret(new Node(RootName));
      if (RootName.empty())
        throw NodeException("Can't get name root element");
      ParseXml(Root, Ret);
      return Ret;
    }

    std::string NodeToUTF8(const Node &node)
    {
      throw NodeException("Not implemented");
      return "";
    }

  }
}
