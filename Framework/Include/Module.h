#ifndef __MODULE_H__
#define __MODULE_H__

#include "IErrorInfoImpl.h"


namespace Common
{
  template <typename TCoClassList>
  class Module
  {
  public:
    static unsigned GetCoClassCount()
    {
      return TypeListLength<TCoClassList>::Len;
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
        if (!TypeListLength<TList>::Len)
          return 0;
        if (Index == index)
        {
          typedef typename TList::Head TItem;
          return TItem::GetUUID();
        }
        return ExtractorCoClassId<Index + 1, typename TList::Tail>::Extract(index);
      }
    };
    template <unsigned Index>
    struct ExtractorCoClassId<Index, NullType>
    {
      static const char* Extract(unsigned)
      {
        return 0;
      }
    };
  };

}

#define DECLARE_MODULE_ENTRY_POINT(module_name_, module_guid_, module_coclasslist_) \
  typedef Common::Module<module_coclasslist_> ModuleType; \
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
  extern "C" bool CreateObject(const char *classId, void **iface) \
  { \
    try \
    { \
      return (ModuleType::CreateObject(classId)).QueryInterface<IFaces::IBase>(reinterpret_cast<IFaces::IBase**>(iface)); \
    } \
    catch (std::exception &) \
    { \
      return false; \
    } \
  } \
  extern "C" unsigned long GetModuleCounter() \
  { \
    try \
    { \
      return Common::ModuleCounter::GetModuleCounter(); \
    } \
    catch (std::exception &) \
    { \
      return static_cast<unsigned long>(-1); \
    } \
  }

#endif  // !__MODULE_H__
