#ifndef __MUTEX_H__
#define __MUTEX_H__

#include "NoCopyable.h"
#include "Exceptions.h"


namespace  System
{
	DECLARE_RUNTIME_EXCEPTION(Mutex)

	class Mutex
		: private Common::NoCopyable
	{
	public:
		Mutex();
		~Mutex();
		void Lock();
		void Unlock();
	private:
		class MutexImpl;
		MutexImpl *Impl;
	};
}

#endif	// !__MUTEX_H__
