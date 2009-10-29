#include "MainDataSrc.h"

IMainDataSrcImpl::IMainDataSrcImpl()
{
}

bool IMainDataSrcImpl::FinalizeCreate()
{
  return true;
}

void IMainDataSrcImpl::BeforeDestroy()
{
}

bool IMainDataSrcImpl::OnInit()
{
  try
  {
    IStorageFileImpl::Init("../../FlashData", false);
  }
  catch (std::exception &)
  {
    return false;
  }
  return true;
}

void IMainDataSrcImpl::OnDone()
{
}

RetCode IMainDataSrcImpl::GetResource(const char *resName, IFaces::IStream **stream)
{
  return OpenStream(resName, stream);
}
