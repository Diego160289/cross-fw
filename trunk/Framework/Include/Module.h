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
      return 0;//TypeListLength<TCoClassList>::Len;
    }
    static const char* GetCoClassId(unsigned index)
    {
      return 0;//ExtractorCoClassId<0, TCoClassList>::Extract(index);
    }
    static RefObjPtr<IFaces::IBase> CreateObject(const char *classId)
    {
      return RefObjPtr<IFaces::IBase>();//ObjectCreator<TCoClassList>::CreateObject(classId);
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

#ifdef _MSC_VER
  #define EXPORT_FUNCTION extern "C" __declspec(dllexport)
#else
  #define EXPORT_FUNCTION extern "C"
#endif

#define DECLARE_MODULE_ENTRY_POINT(module_name_, module_guid_, module_coclasslist_) \
  typedef Common::Module<module_coclasslist_> ModuleType; \
  EXPORT_FUNCTION const char* GetModuleName() \
  { \
    return module_name_; \
  } \
  EXPORT_FUNCTION const char* GetModuleGuid() \
  { \
    return #module_guid_; \
  } \
  EXPORT_FUNCTION unsigned GetCoClassCount() \
  { \
    return ModuleType::GetCoClassCount(); \
  } \
  EXPORT_FUNCTION const char* GetCoClassId(unsigned index) \
  { \
    return ModuleType::GetCoClassId(index); \
  } \
  EXPORT_FUNCTION bool CreateObject(const char *classId, void **iface) \
  { \
    try \
    { \
      Common::RefObjPtr<IFaces::IBase> Obj(ModuleType::CreateObject(classId)); \
      if (!Obj.Get()) \
        return false; \
      return Obj->QueryInterface(IFaces::IBase::GetUUID(), iface) == IFaces::retOk; \
    } \
    catch (std::exception &) \
    { \
      return false; \
    } \
  } \
  EXPORT_FUNCTION unsigned long GetModuleCounter() \
  { \
    try \
    { \
      return Common::ModuleCounter::GetModuleCounter(); \
    } \
    catch (std::exception &) \
    { \
      return static_cast<unsigned long>(-1); \
    } \
  } \

#endif  // !__MODULE_H__
