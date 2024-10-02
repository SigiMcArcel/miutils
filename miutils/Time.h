#pragma once
#include <time.h>
#include <sys/time.h>
#include <stdio.h>

namespace miutils
{

	class Time
	{
		long _LastTick;
		long _StarTick;
	public:


		Time()
			:_LastTick(-1)
			, _StarTick(-1)
		{
		}


		~Time()
		{
		}

		bool reset()
		{
			_LastTick = -1;
			_StarTick = -1;
			return true;
		}

	

		bool elapsed(long millisecond)
		{
			long acttime = 0;
			long diff = 0;
			
			if (millisecond <= 0)
			{
				_StarTick = -1;
				_LastTick = -1;
				return false;
			}
		
			if (_StarTick == -1)
			{
				_LastTick =_StarTick = getMsTickCount();
			}
			else
			{
				acttime = getMsTickCount();
				diff = acttime - _StarTick;
				_LastTick = acttime;
				if (diff > millisecond)
				{
					_StarTick = -1;
					return true;
				}
			}
			return false;

		}
		long getMsTickCount()
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

