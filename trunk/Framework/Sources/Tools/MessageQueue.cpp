#include "MessageQueue.h"


const char* IMessageQueueManagerImpl::CreateMessageQueue()
{
  return 0;
}

bool IMessageQueueManagerImpl::DestroyMessageQueue(const char *queueId)
{
  return false;
}

bool IMessageQueueManagerImpl::QueryMessageQueue(const char *queueId, IFaces::IMessageQueue **messageQueue)
{
  return false;
}

bool IMessageQueueManagerImpl::EnumMessageQueues(IFaces::IEnum **messageQuques)
{
  return false;
}
