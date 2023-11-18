#pragma once
#include <mutex>
#include <pthread.h>
namespace miutils
{
	class CriticalSection
	{
	private:
		pthread_mutex_t _CriticalSection;
	public:
		CriticalSection()
		{
			pthread_mutex_init(&_CriticalSection, NULL);
		}
		~CriticalSection()
		{
			pthread_mutex_destroy(&_CriticalSection);
		}
		void EnterCriticalSection()
		{
			pthread_mutex_lock(&_CriticalSection);
		}
		void LeaveCriticalSection()
		{
			pthread_mutex_unlock(&_CriticalSection);
		}
	};
}

