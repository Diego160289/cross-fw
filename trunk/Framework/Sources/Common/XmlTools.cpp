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

    std::string NodeToUTF8(const Node &node, bool addHeader)
    {
      struct
      {
        void operator () (const Node &srcNode, TiXmlNode *tiXml)
        {
          TiXmlElement Item(srcNode.GetNodeName());
          if (srcNode.HasProperty())
          {
            const PropertyMap &Props = srcNode.GetPropertiesMap();
            for (PropertyMap::const_iterator i = Props.begin() ; i != Props.end() ; ++i)
              Item.SetAttribute(i->first, i->second);
          }
          if (srcNode.HasValue())
          {
            const std::wstring &Value = srcNode.GetValue();
            std::string Utf8Value;
            utf8::utf16to8(Value.begin(), Value.end(), std::back_inserter(Utf8Value));
            TiXmlText Text(Utf8Value);
            Text.SetCDATA(true);
            if (!Item.InsertEndChild(Text))
              throw NodeException("Can't add xml node");
          }
          if (srcNode.HasChildList())
          {
            const NodeList &ChildList = srcNode.GetChildNodes();
            for (NodeList::const_iterator i = ChildList.begin() ; i != ChildList.end() ; ++i)
              (*this)(*i->Get(), &Item);
          }
          if (!tiXml->InsertEndChild(Item))
            throw NodeException("Can't add xml node");
        }
      } NodeToTiXml;
      struct
      {
        std::string operator () (const TiXmlNode *tiNode, int depth)
        {
          struct
          {
            std::string operator () (const TiXmlNode *valueNode)
            {
              return valueNode->Type() == TiXmlNode::TEXT ?
                "<![CDATA[" + valueNode->ValueStr() + "]]>" :
                valueNode->ValueStr();
            }
          } ValueToText;
          std::string RetStr;
          for (int i = 0 ; i < depth ; ++i)
            RetStr += " ";
          std::string BeginTag, BeginTagEx, EndTag, EndTagEx;
          if (tiNode->Type() == TiXmlNode::TEXT)
            BeginTag = BeginTagEx = EndTag = EndTag = "";
          else
          {
            BeginTag = "<";
            BeginTagEx = "</";
            EndTag = ">";
            EndTagEx = "/>";
          }
          RetStr += BeginTag + ValueToText(tiNode);
          const TiXmlElement *Element = tiNode->ToElement();
          if (Element)
          {
            for (const TiXmlAttribute *i = Element->FirstAttribute() ; i ; i = i->Next())
            {
              RetStr += " ";
              RetStr += i->Name();
              RetStr += " = \"" + i->ValueStr() + "\"";
            }
          }
          if (!tiNode->FirstChild())
            RetStr += EndTagEx;
          else
            if (tiNode->FirstChild() == tiNode->LastChild() && tiNode->FirstChild()->ToText())
            {
              RetStr += EndTag;
              RetStr += (*this)(tiNode->FirstChild(), depth + 1);
              RetStr += BeginTagEx + ValueToText(tiNode) + EndTag;
            }
            else
            {
              RetStr += ">";
              for (const TiXmlNode *node = tiNode->FirstChild() ; node ; node = node->NextSibling())
              {
                if (!node->ToText())
                  RetStr += "\n";
                RetStr += (*this)(node, depth + 1);
              }
              RetStr += "\n";
              for(int i = 0 ; i < depth ; ++i)
                RetStr += " ";
              RetStr += BeginTagEx + ValueToText(tiNode) + EndTag;
            }
          return RetStr;
        }
      } TiXmlToString;
      TiXmlDocument  Doc;
      NodeToTiXml(node, &Doc);
      const TiXmlNode *TiXml = Doc.RootElement();
      if (!TiXml)
        throw NodeException("Empty xml document");
      return (addHeader ? "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" : "") +
        TiXmlToString(TiXml, 0);
    }

  }
}
