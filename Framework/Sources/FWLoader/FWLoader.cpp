#include <iostream>

#include "IErrorInfoImpl.h"

#include "Mutex.h"

namespace Common
{
  template
  <
    const char *ModuleGuid,
    const char *ModuleName,
    typename TCoClassList
  >
  class Module
  {
  public:
    static const char* GetModuleGuid()
    {
      return ModuleGuid;
    }
    static const char* GetModuleName()
    {
      return ModuleName;
    }
    static unsigned GetCoClassCount()
    {
      return TypeListLength<TCoClassList>::Len;
    }
    static const char* GetCoClassId(unsigned index)
    {
      return ExtractorCoClassId<0, TCoClassList>::Extract(index);
    }
    /*template <typename T>
    static IFacesTools::RefObjPtr<T> CreateObject(const char *classId)
    {
      return 0;
    }*/
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
          return TItem::GetCoClassId();
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

extern const char ModuleGuid[] = "1";
extern const char ModuleName[] = "2";

int main()
{
  typedef IFacesImpl::IErrorInfoImpl<IFacesTools::MultiObject, System::Mutex> ErType;
  typedef Common::Module<ModuleGuid, ModuleName, TYPE_LIST_1(ErType)> a;
  std::cout
    << a::GetCoClassId(0) << " "
    << std::endl;
  /*{
    IFacesTools::RefObjPtr<IFaces::IErrorInfo> Ie =
      ErType::CreateObject();
    Ie->AddError(0, "");
  }
  std::cout << (int) IFacesTools::ModuleCounter::GetModuleCounter() << std::endl;*/
  return 0;
}
