#include <sched.h>
#include "/Timer.h"


void * miutils::Timer::Proc(void * p)
{
	Timer* timer = reinterpret_cast<Timer*>(p);
	
	while (timer->_TimerState == 1)
	{
		timer->Lock();
		timer->setEvent(timer, timer->_Name);
		timer->Unlock();
		::usleep(timer->GetInterval() * 1000);
	}
	return nullptr;
}

/// <summary>
/// Starts the timer
/// </summary>
/// <param name="intervall">Intervall in millsecond</param>
/// <returns></returns>
miutils::TimerResults miutils::Timer::Start(int intervall)
{
	return Start(intervall, nullptr, 0, Schedulers::Other);
}

miutils::TimerResults miutils::Timer::Start(int intervall, void* obj)
{
	return Start(intervall,obj, 0, Schedulers::Other);
}

miutils::TimerResults miutils::Timer::Start(int intervall, void* obj, int prio, Schedulers schedulerType)
{
	struct sched_param params;
	int err = 0;
	if (_TimerState != 0)
	{
		return TimerResults::ErrorAllreadyRunning;
	}
	_Intervall = intervall;
	_TimerState = 1;
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
		if (prio < params.sched_priority)
		{
			return TimerResults::ErrorParam;
		}
		params.sched_priority = prio;
	}
	
	_Object = obj;

	err = pthread_create(&_Thread, NULL, Timer::Proc, this);
	if (err != 0)
	{
		return TimerResults::ErrorCreate;
	}
	err = pthread_setschedparam(_Thread, static_cast<int>(schedulerType), &params);
	if (err != 0) {

		return TimerResults::ErrorCreate;
	}

	return TimerResults::ErrorOk;
}

miutils::TimerResults miutils::Timer::Stop()
{
	_TimerState = 0;
	if (_Thread != 0)
	{
		pthread_join(_Thread, NULL);
		_Thread = 0;
	}
	return TimerResults::ErrorOk;
}

long miutils::Timer::GetNumOfCPU()
{
	return sysconf(_SC_NPROCESSORS_ONLN);;
}

int miutils::Timer::GetCPUAffinity()
{
	cpu_set_t cpuset;
	int result = pthread_getaffinity_np(_Thread, sizeof(cpu_set_t), &cpuset);
	if (result != 0)
	{
		return result;
	}
	return 0;
}

int miutils::Timer::SetCPUAffinity(int affinity)
{
	return 0;
}

miutils::Schedulers miutils::Timer::GetScheduler()
{
	return miutils::Schedulers();
}

int miutils::Timer::SetScheduler(miutils::Schedulers scheduler)
{
	return 0;
}

int miutils::Timer::GetPriority()
{
	return 0;
}

int miutils::Timer::SetPriority(int priority)
{
	return 0;
}

void miutils::Timer::Lock()
{
	_CriticalSection.EnterCriticalSection();
}
void miutils::Timer::Unlock()
{
	_CriticalSection.LeaveCriticalSection();
}