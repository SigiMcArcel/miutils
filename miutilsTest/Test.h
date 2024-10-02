#pragma once
#include <mi/miutils/Timer.h>


using namespace miutils;

class Test : public EventListener
{
private:
	Timer _timer;

public:
	Test(const std::string name)
		:_timer(name,this)
	{

	}
	// Geerbt über iEventListener
	virtual void eventOccured(void* sender, const std::string& name) override;
	void start(int intervall)
	{
		_timer.Start(intervall);
	}

	void stop()
	{
		_timer.Stop();
	}
};

