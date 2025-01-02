#include <sched.h>
#include "Timer.h"


void * miutils::Timer::Proc(void * p)
{
	Timer* timer = reinterpret_cast<Timer*>(p);
	
	if (timer == nullptr)
	{
		return nullptr;
	}
	while (1)
	{
		if (!timer->setEvent())
		{
			return nullptr;
		}
	}
	return nullptr;
}

miutils::TimerResults miutils::Timer::Start()
{
	return Start(_Intervall, _Prority, _SchedulerType);
}

/// <summary>
/// Starts the timer with default scheduler
/// </summary>
/// <param name="intervall">Intervall in millsecond</param>
/// <returns></returns>
miutils::TimerResults miutils::Timer::Start(int intervall)
{
	return Start(intervall, 0, SchedulerType::Other);
}

miutils::TimerResults miutils::Timer::Start(int intervall, int prio, SchedulerType schedulerType)
{
	struct sched_param params;
	int err = 0;
	if (_TimerState != 0)
	{
		return TimerResults::ErrorAllreadyRunning;
	}
	
	_Intervall = intervall;
	if (_Intervall <= 0)
	{
		return miutils::TimerResults::ErrorParam;
	}
	_TimerState = 1;
	if(schedulerType == SchedulerType::Other)
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
	cpu_set_t cpuset;
	if (affinity >= GetNumOfCPU())
	{
		return -1;
	}
	CPU_ZERO(&cpuset);          // Initialisiert den CPU-Satz
	CPU_SET(affinity, &cpuset);        // Setzt den Thread auf CPU 2
	int result = pthread_setaffinity_np(_Thread, sizeof(cpu_set_t), &cpuset);
	if (result != 0)
	{
		return -1;
	}
	return 0;
}

miutils::SchedulerType miutils::Timer::GetScheduler()
{
	return miutils::SchedulerType();
}

int miutils::Timer::SetScheduler(miutils::SchedulerType scheduler)
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

void miutils::Timer::addListener(miutils::TimerEventListenerShared listener)
{
	_Listeners.push_back(listener);
}

void miutils::Timer::addListener(miutils::TimerEventListenerShared listener, miutils::TimerEventListenerObject object)
{
	listener->setObject(object);
	_Listeners.push_back(listener);
}

void miutils::Timer::removeListener(miutils::TimerEventListenerShared listener)
{
	_Listeners.remove(listener);
}

const std::string miutils::Timer::name() const
{
	return _Name;
}
bool miutils::Timer::setEvent()
{
	bool stop = false;
	_CriticalSection.EnterCriticalSection();
	for (const auto& listener : _Listeners) {
		if (listener->timerEventOccured(listener->getObject(), _Name))
		{
			fprintf(stderr, "Timer name %s leaves\n", _Name.c_str());
			_TimerState = 0;
			_Thread = 0;
			_CriticalSection.LeaveCriticalSection();
			return false;
		}
	}
	
	
	if (_Intervall > 0)
	{
		::usleep(_Intervallus);
	}
	_CriticalSection.LeaveCriticalSection();
	if (_TimerState == 1)
	{
		return true;
	}
	return false;
}