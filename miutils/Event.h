#pragma once
#include <list>
#include <string>

//class iEventListener;

namespace miutils
{
	class EventListener
	{
	public:
		virtual void eventOccured(void* sender, const std::string& name) = 0;
	};

	class EventHandler
	{
	protected:
		std::list<EventListener*> listeners;
	public:
		EventHandler()
			:listeners()
		{

		}
		void addListener(EventListener* listener)
		{
			listeners.push_back(listener);
		}
		void removeListener(EventListener* listener)
		{
			listeners.remove(listener);
		}
		void setEvent(void* sender, const std::string& event)
		{
			std::list<EventListener*>::iterator it;
			for (it = listeners.begin(); it != listeners.end(); ++it) {
				(*it)->eventOccured(sender, event);
			}
		}
	};
}



