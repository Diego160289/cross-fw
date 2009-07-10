#include "MessageQueue.h"


const char* CALL_TYPE IMessageQueueManagerImpl::CreateMessageQueue()
{
  return 0;
}

bool CALL_TYPE IMessageQueueManagerImpl::DestroyMessageQueue(const char *queueId)
{
  return false;
}

bool CALL_TYPE IMessageQueueManagerImpl::QueryMessageQueue(const char *queueId, IFaces::IMessageQueue **messageQueue)
{
  return false;
}

bool CALL_TYPE IMessageQueueManagerImpl::EnumMessageQueues(IFaces::IEnum **messageQuques)
{
  return false;
}
