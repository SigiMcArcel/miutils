#include <sched.h>
#include "Timer.h"

void * miutils::Timer::proc(void * p)
{
	Timer* timer = reinterpret_cast<Timer*>(p);
	
	while (timer->_timerState == 1)
	{
		timer->setEvent(timer, timer->_name);
		usleep(timer->getInterval() * 1000);
	}
	return nullptr;
}

/// <summary>
/// Starts the timer
/// </summary>
/// <param name="intervall">Intervall in millsecond</param>
/// <returns></returns>
miutils::TimerResults miutils::Timer::start(int intervall)
{
	return start(intervall, nullptr, 0, Schedulers::Other);
}

miutils::TimerResults miutils::Timer::start(int intervall, void* obj)
{
	return start(intervall,obj, 0, Schedulers::Other);
}

miutils::TimerResults miutils::Timer::start(int intervall, void* obj, int prio, Schedulers schedulerType)
{
	struct sched_param params;
	int err = 0;
	if (_timerState != 0)
	{
		return TimerResults::ErrorAllreadyRunning;
	}
	_intervall = intervall;
	_timerState = 1;
	if(schedulerType == Schedulers::Other)
	{
		params.sched_priority = 0;
	}
	else
	{
		params.sched_priority = sched_get_priority_max(static_cast<int>(schedulerType));
		if (prio > params.sched_priority)
		{
			return TimerResults::ErrorParam;
		}
		params.sched_priority = sched_get_priority_min(static_cast<int>(schedulerType));
		if (prio > params.sched_priority)
		{
			return TimerResults::ErrorParam;
		}
		params.sched_priority = prio;
	}
	
	_Object = obj;

	err = pthread_create(&_thread, NULL, Timer::proc, this);
	if (err != 0)
	{
		return TimerResults::ErrorCreate;
	}
	err = pthread_setschedparam(_thread, static_cast<int>(schedulerType), &params);
	if (err != 0) {

		return TimerResults::ErrorCreate;
	}

	pthread_join(_thread, NULL);
	return TimerResults::ErrorOk;
}

miutils::TimerResults miutils::Timer::stop()
{
	_timerState = 0;
	return TimerResults::ErrorOk;
}

long miutils::Timer::getNumOfCPU()
{
	return sysconf(_SC_NPROCESSORS_ONLN);;
}

int miutils::Timer::getCPUAffinity()
{
	cpu_set_t cpuset;
	int result = pthread_getaffinity_np(_thread, sizeof(cpu_set_t), &cpuset);
	if (result != 0)
	{
		return result;
	}
	return 0;
}

int miutils::Timer::setCPUAffinity(int affinity)
{
	return 0;
}

miutils::Schedulers miutils::Timer::getScheduler()
{
	return miutils::Schedulers();
}

int miutils::Timer::setScheduler(miutils::Schedulers scheduler)
{
	return 0;
}

int miutils::Timer::getPriority()
{
	return 0;
}

int miutils::Timer::setPriority(int priority)
{
	return 0;
}
