#ifndef __MESSAGEQUEUE_H__
#define __MESSAGEQUEUE_H__

#include "IFacesTools.h"
#include "Mutex.h"

class IMessageQueueManagerImpl
  : public Common::CoClassBase
      <
        TYPE_LIST_1(IFaces::IMessageQueueManager)
      >
{
public:
  DECLARE_UUID(9a366c11-d401-41ce-8e05-ff5bc27bc16f)

  // IMessageQueueManager
  virtual const char* CreateMessageQueue();
  virtual bool DestroyMessageQueue(const char *queueId);
  virtual bool QueryMessageQueue(const char *queueId, IFaces::IMessageQueue **messageQueue);
  virtual bool EnumMessageQueues(IFaces::IEnum **messageQuques);

private:
};

#endif  // !__MESSAGEQUEUE_H__
