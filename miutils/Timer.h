#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <memory>
#include <list>
#include <any>
#include "CriticalSection.h"

namespace miutils
{
	typedef enum class SchedulerType_e
	{
		Other =	0,
		Fifo =	1,
		RoundRobin = 2,
		None = -1
	}SchedulerType;

	typedef enum TimerResults_e
	{
		ErrorOk,
		ErrorCreate,
		ErrorParam,
		ErrorAllreadyRunning
	}TimerResults;

	class MiAny {
		// Diese Klasse kann als Basis verwendet werden
	};

	typedef void* TimerEventListenerObject;

	class TimerEventListener
	{
	private:
		TimerEventListenerObject _Object;
	public:
		TimerEventListener()
			:_Object(nullptr)
		{

		}
		TimerEventListener(TimerEventListenerObject object)
			:_Object(object)
		{

		}
		TimerEventListenerObject getObject() const
		{
			return _Object;
		}
		void setObject(TimerEventListenerObject object)
		{
			_Object = object;
		}

		virtual void timerEventOccured(miutils::TimerEventListenerObject, const std::string& name) = 0;
	};

	typedef TimerEventListener* TimerEventListenerShared;

	class Timer
	{

	private:
		std::string _Name;
		int _Intervall;
		int _Intervallus;
		int _CPUAffinity;
		int _Prority;
		SchedulerType _SchedulerType;
		pthread_t _Thread;
		int _Thread_id;
		int _TimerState;
		CriticalSection _CriticalSection;
		long _NumOfCPU;
		std::list<TimerEventListenerShared> _Listeners;

		static void* Proc(void* p);
		
		const SchedulerType defaultScheduler = SchedulerType::Other;

		long GetNumOfCPU();
		int GetCPUAffinity();
		int SetCPUAffinity(int affinity);
		SchedulerType GetScheduler();
		int SetScheduler(miutils::SchedulerType scheduler);
		int GetPriority();
		int SetPriority(int priority);

	public:
		Timer(const std::string& name, miutils:: TimerEventListenerShared listener, int Intervall)
			: _Name(name)
			, _Intervall(Intervall)
			, _Intervallus(_Intervall * 1000)
			, _CPUAffinity(-1)
			, _Prority(0)
			, _SchedulerType(defaultScheduler)
			, _Thread(0)
			, _Thread_id(0)
			, _TimerState(0)
			, _CriticalSection(CriticalSection())
			, _NumOfCPU(0)
			, _Listeners()
		{
			addListener(listener);
		};

		Timer(const std::string& name, miutils::TimerEventListenerShared listener)
			: _Name(name)
			, _Intervall(0)
			, _Intervallus(_Intervall * 1000)
			, _CPUAffinity(-1)
			, _Prority(0)
			, _SchedulerType(defaultScheduler)
			, _Thread(0)
			, _Thread_id(0)
			, _TimerState(0)
			, _CriticalSection(CriticalSection())
			, _NumOfCPU(0)
			, _Listeners()
		{
			addListener(listener);
		};

		Timer(const std::string& name, int intervall, int cpuAffinity,
			int priority, SchedulerType schedulerType, miutils::TimerEventListenerShared listener)
			: _Name(name)
			, _Intervall(intervall)
			, _Intervallus(_Intervall * 1000)
			, _CPUAffinity(cpuAffinity)
			, _Prority(priority)
			, _SchedulerType(schedulerType)
			, _Thread(0)
			, _Thread_id(0)
			, _TimerState(0)
			, _CriticalSection(CriticalSection())
			, _NumOfCPU(0)
		{
			addListener(listener);
		};

		Timer(const std::string& name, int intervall, int cpuAffinity,
			int priority, SchedulerType schedulerType)
			: _Name(name)
			, _Intervall(intervall)
			, _Intervallus(_Intervall * 1000)
			, _CPUAffinity(cpuAffinity)
			, _Prority(priority)
			, _SchedulerType(schedulerType)
			, _Thread(0)
			, _Thread_id(0)
			, _TimerState(0)
			, _CriticalSection(CriticalSection())
			, _NumOfCPU(0)
		{
			
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

		void addListener(miutils::TimerEventListenerShared listener);
		void addListener(miutils::TimerEventListenerShared listener, miutils::TimerEventListenerObject object);
		void removeListener(miutils::TimerEventListenerShared listener);
		virtual bool setEvent();

		TimerResults Start();
		TimerResults Start(int intervall);
		TimerResults Start(int intervall,int prio,SchedulerType schedulerType);
		TimerResults Stop();

		const CriticalSection& criticalSection() const
		{
			return _CriticalSection;
		}
		void Lock();
		void Unlock();
		const std::string name() const;
	};
}