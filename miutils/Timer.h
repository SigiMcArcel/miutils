#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "Event.h"
#include "CriticalSection.h"

namespace miutils
{
	typedef enum Schedulers_e
	{
		Other =	0,
		Fifo =	1,
		RoundRobin = 2,
		None = -1
	}Schedulers;

	typedef enum TimerResults_e
	{
		ErrorOk,
		ErrorCreate,
		ErrorParam,
		ErrorAllreadyRunning
	}TimerResults;

	class Timer : public EventHandler
	{

	public:

		Timer(const std::string& name, EventListener* listener)
			:_Intervall(0)
			, _Thread_id(0)
			,_Thread(0)
			, _TimerState(0)
			, _Name(name)
			, _CriticalSection()
		{

			addListener(listener);
		};

		~Timer()
		{
			_TimerState = 0;
			if (_Thread != 0)
			{
				pthread_join(_Thread, NULL);
			}
			
		}
		int GetInterval()
		{
			return _Intervall;
		}

		TimerResults Start(int intervall);
		TimerResults Start(int intervall,void* obj);
		TimerResults Start(int intervall, void* obj, int prio,Schedulers schedulerType);
		TimerResults Stop();

		void* GetObject() { return _Object; };
		void Lock();
		void Unlock();

	private:
		int _Intervall;
		static void* Proc(void* p);
		int _Thread_id;
		pthread_t _Thread;
		int _TimerState;
		std::string _Name;
		long _NumOfCPU;
		int _CPUAffinity;
		int _Prority;
		void* _Object;
		CriticalSection _CriticalSection;


		long GetNumOfCPU();
		int GetCPUAffinity();
		int SetCPUAffinity(int affinity);
		Schedulers GetScheduler();
		int SetScheduler(miutils::Schedulers scheduler);
		int GetPriority();
		int SetPriority(int priority);
		

	};
}