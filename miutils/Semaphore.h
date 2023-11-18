#pragma once
#include <semaphore.h> 
#include <errno.h>
namespace miutils
{
	class Semaphore
	{
	private:
		sem_t _Semaphore;

	public:
		Semaphore()
			:_Semaphore()
		{
			::sem_init(&_Semaphore, 0, 1);
		}
		~Semaphore()
		{
			::sem_destroy(&_Semaphore);
		}

		void set()
		{
			sem_post(&_Semaphore);
		}

		bool wait(long timeoutMs)
		{
			struct timespec t;
			t.tv_nsec = timeoutMs * 1000 * 1000;
			if (timeoutMs == 0)
			{
				::sem_wait(&_Semaphore);
				return true;
			}
			if (::sem_timedwait(&_Semaphore, &t) == -1)
			{
				return false;
			}
			return true;
		}
	};
}

