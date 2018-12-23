#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H
#include <map>
#include <memory>

class Listner;
class EventHandler
{
public:
	enum class Event{Notification, SoundEffect, NumEvents};
	void notify(Event event);
	Listner* getListner(Event event);
private:
	void addListner(Event event);
	void removeListner(Event event);
	
	std::map<Event, std::unique_ptr<Listner>> listners;

};
#endif // !EVENTHANDLER_H
