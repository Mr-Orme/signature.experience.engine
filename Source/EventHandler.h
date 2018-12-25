#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H
#include <map>
#include <memory>

#include "Listner.h"
class EventHandler
{
public:
	EventHandler();
	enum class Event{
		CreateObject, 
		Notification, 
		SoundEffect, 
		UserInput,
		NumEvents};
	void notify(Event event, void* data);
	Listner* getListner(Event event);
private:
	void addListner(Event event);
	void removeListner(Event event);
	
	std::map<Event, std::unique_ptr<Listner>> listners;

};
#endif // !EVENTHANDLER_H
