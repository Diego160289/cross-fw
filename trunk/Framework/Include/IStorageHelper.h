#ifndef __ISTORAGEHELPER_H__
#define __ISTORAGEHELPER_H__

#include "IFaces.h"
#include "RefObjPtr.h"
#include "Exceptions.h"
#include "IEnumImpl.h"

#include <string>


namespace IFacesImpl
{

  DECLARE_RUNTIME_EXCEPTION(ISorageHelper)

  class ISorageHelper
  {
  public:
    typedef Common::RefObjPtr<IFaces::IStorage> IStoragePtr;
    typedef Common::RefObjPtr<IFaces::IStream> IStreamPtr;

    ISorageHelper(IStoragePtr storage);

    IStoragePtr CreateStorage(const std::string &name);
    IStoragePtr OpenStorage(const std::string &name);
    IStreamPtr CreateStream(const std::string &name);
    IStreamPtr OpenStream(const std::string &name);
    void RemoveItem(const std::string &name);
    IEnumHelper Enum() const;

  private :
    IStoragePtr Storage;
  };

}

#endif // !__ISTORAGEHELPER_H__
