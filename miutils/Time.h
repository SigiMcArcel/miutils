#pragma once
#include <time.h>
#include <sys/time.h>
#include <stdio.h>

namespace miutils
{

	class Time
	{
		unsigned long _timerCount;
		unsigned long _startime;
	public:


		Time()
			:_timerCount(0)
		{
		}


		~Time()
		{
		}

		bool reset()
		{
			_timerCount = getMsTickCount();
			_startime = _timerCount;
			return true;
		}
		bool wait(unsigned long millsecond)
		{
			unsigned long acttime = getMsTickCount();
			unsigned long diff = acttime - _timerCount;

			if (_timerCount == 0)
			{
				return false;
			}
			if (diff >= millsecond)
			{
				_timerCount = 0;
				return true;
			}

			return false;
		}

		unsigned long getMsTickCount()
		{
			long tick = 0;
			struct timespec ts;
			//timespec_get(&ts, TIME_UTC);
			clock_gettime(CLOCK_REALTIME, &ts);
			tick = (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
			return tick;
			//struct timeval tv;
			//if (gettimeofday(&tv, NULL) != 0) return 0;
			//return (unsigned long)((tv.tv_sec * 1000ul) + (tv.tv_usec / 1000ul));


		}

	};
}

