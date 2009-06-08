#ifndef __MUTEXSTUB_H__
#define __MUTEXSTUB_H__

#include "NoCopyable.h"


namespace System
{
	class MutexStub
		: private Common::NoCopyable
	{
	public:
		void Lock()
		{
		}
		void Unlock()
		{
		}
	};
}

#endif	// !__MUTEXSTUB_H__
