#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "Event.h"

namespace miutils
{
	typedef enum Schedulers_e
	{
		Other	=	0,
		Fifo	=	1,
		RoundRobin	=	2
	}Schedulers;

	class Timer : public EventHandler
	{

	public:

		Timer(const std::string& name, EventListener* listener)
			:_intervall(0)
			, _thread_id(0)
			, _timerState(0)
			, _name(name)
		{

			addListener(listener);
		};


		int getInterval()
		{
			return _intervall;
		}

		int start(int intervall);
		void stop();


	private:
		int _intervall;
		static void* proc(void* p);
		int _thread_id;
		pthread_t _thread;
		int _timerState;
		std::string _name;
		long _numOfCPU;
		int _CPUAffinity;
		int _Prority;


	private:
		long getNumOfCPU();
		int getCPUAffinity();
		int setCPUAffinity(int affinity);
		miutils::Schedulers getScheduler();
		int setScheduler(miutils::Schedulers scheduler);
		int getPriority();
		int setPriority(int priority);

	};
}