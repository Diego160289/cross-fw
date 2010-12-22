//============================================================================
// Date        : 22.12.2010
// Author      : Tkachenko Dmitry
// Copyright   : (c) Tkachenko Dmitry (TkachenkoDmitryV@gmail.com)
//============================================================================


#ifndef __NAMEDVARMAP_H__
#define __NAMEDVARMAP_H__

#include "Variant.h"
#include "Exceptions.h"

#include <map>


namespace Common
{

  template <typename TName, typename TValue>
  class VarName
  {
  public:
    VarName(const TName &name)
      : Name(name)
    {
    }
    const TName& GetName() const
    {
      return Name;
    }
  private:
    TName Name;
  };

  DECLARE_LOGIC_EXCEPTION(NamedVarMap)

  template <typename TName>
  class NamedVarMap
  {
  public:
    template <typename TValue>
    void Set(const VarName<TName, TValue> &name, const TValue &value)
    {
      Variables[name.GetName()] = value;
    }
    template <typename TValue>
    void Set(const TName &name, const TValue &value)
    {
      Set(VarName<TName, TValue>(name), value);
    }
    template <typename TValue>
    TValue& Get(const VarName<TName, TValue> &name) const
    {
      typename VariantPool::iterator Iter = const_cast<NamedVarMap<TName> *>(this)->Variables.find(name.GetName());
      if (Iter == Variables.end())
        throw NamedVarMapException("Variable not found");
      return (TValue&)Iter->second;
    }
    template <typename TValue>
    TValue& Get(const TName &name) const
    {
      return Get(VarName<TName, TValue>(name));
    }
    template <typename TValue>
    void Remove(const VarName<TName, TValue> &name)
    {
      typename VariantPool::iterator Iter = Variables.find(name.GetName());
      if (Iter == Variables.end())
        throw NamedVarMapException("Variable not found");
      Variables.erase(Iter);
    }
    void Remove(const TName &name)
    {
      typename VariantPool::iterator Iter = Variables.find(name);
      if (Iter == Variables.end())
        throw NamedVarMapException("Variable not found");
      Variables.erase(Iter);
    }
    bool IsEmpty() const
    {
      return Variables.empty();
    }
    template <typename TValue>
    bool ExistsVariable(const VarName<TName, TValue> &name) const
    {
      return Variables.find(name.GetName()) != Variables.end();
    }
    bool ExistsVariable(const TName &name) const
    {
      return Variables.find(name) != Variables.end();
    }
    void Clear()
    {
      Variables.clear();
    }
  private:
    typedef std::map<TName, Variant> VariantPool;
    VariantPool Variables;
  public:
    typedef typename VariantPool::const_iterator ConstIterator;
    ConstIterator Begin() const
    {
      return Variables.begin();
    }
    ConstIterator End() const
    {
      return Variables.end();
    }
  };

}

#endif  // !__NAMEDVARMAP_H__
