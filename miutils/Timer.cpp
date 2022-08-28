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

int miutils::Timer::start(int intervall)
{
	struct sched_param params;
	int err = 0;
	if (_timerState != 0)
	{
		return 1;
	}
	_intervall = intervall;
	_timerState = 1;
	params.sched_priority = sched_get_priority_max(SCHED_FIFO);

	err = pthread_create(&_thread, NULL, Timer::proc, this);
	if (err != 0)
	{
		return err;
	}
	err = pthread_setschedparam(_thread, SCHED_FIFO, &params);
	if (err != 0) {
		
		return err;
	}

	pthread_join(_thread, NULL);
	return 0;
}

void miutils::Timer::stop()
{
	_timerState = 0;
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
