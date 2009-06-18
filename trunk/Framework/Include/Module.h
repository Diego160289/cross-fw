#ifndef __MODULE_H__
#define __MODULE_H__

#include "IErrorInfoImpl.h"

namespace IFacesTools
{
  template <typename TCoClassList>
  class Module
  {
  public:
    static unsigned GetCoClassCount()
    {
      return Common::TypeListLength<TCoClassList>::Len;
    }
    static const char* GetCoClassId(unsigned index)
    {
      return ExtractorCoClassId<0, TCoClassList>::Extract(index);
    }
    static RefObjPtr<IFaces::IBase> CreateObject(const char *classId)
    {
      return ObjectCreator<TCoClassList>::CreateObject(classId);
    }
  private:
    Module();
    ~Module();
    template <unsigned Index, typename TList>
    struct ExtractorCoClassId
    {
      static const char* Extract(unsigned index)
      {
        if (!Common::TypeListLength<TList>::Len)
          return 0;
        if (Index == index)
        {
          typedef typename TList::Head TItem;
          return TItem::GetCoClassId();
        }
        return ExtractorCoClassId<Index + 1, typename TList::Tail>::Extract(index);
      }
    };
    template <unsigned Index>
    struct ExtractorCoClassId<Index, Common::NullType>
    {
      static const char* Extract(unsigned)
      {
        return 0;
      }
    };
  };

}

#define DECLARE_MODULE_ENTRY_POINT(module_name_, module_guid_, module_coclasslist_) \
  typedef IFacesTools::Module<module_coclasslist_> ModuleType; \
  extern "C" const char* GetModuleName() \
  { \
    return module_name_; \
  } \
  extern "C" const char* GetModuleGuid() \
  { \
    return #module_guid_; \
  } \
  extern "C" unsigned GetCoClassCount() \
  { \
    return ModuleType::GetCoClassCount(); \
  } \
  extern "C" const char* GetCoClassId(unsigned index) \
  { \
    return ModuleType::GetCoClassId(index); \
  } \
  extern "C" void* CreateObject(const char *classId) \
  { \
    return ModuleType::CreateObject(classId).Detach(); \
  } \
  extern "C" unsigned long GetModuleCounter() \
  { \
    return IFacesTools::ModuleCounter::GetModuleCounter(); \
  }

#endif  // !__MODULE_H__
