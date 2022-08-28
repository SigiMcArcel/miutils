#pragma once
#include "Timer.h"
#include "Event.h"

using namespace miutils;

class Test : public EventListener
{
private:
	Timer _timer;

public:
	Test()
		:_timer("Test",this)
	{

	}
	// Geerbt �ber iEventListener
	virtual void eventOccured(void* sender, const std::string& name) override;
	void start(int intervall)
	{
		_timer.start(intervall);
	}

	void stop()
	{
		_timer.stop();
	}
};

