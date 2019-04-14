#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H
#include <map>
#include <memory>
#include <variant>
#include <vector>


#include "InputDevice.h"
#include "Vector2D.h"
#include "Initializers.h"
//struct ObjectFactoryPresets;
class Listner;
class IEventTrigger;
class Object;
class EventHandler
{
public:
	
	EventHandler();
	~EventHandler();
	enum class Event{
		CreateObject, 
		Notification, 
		SoundEffect, 
		UserInput,
		AdjustView,
		NumEvents};
	using EventData = std::variant<IEventTrigger*, Vector2D, const InputDevice::UserInputs, ObjectFactoryPresets* > ;
	///Vector2D --> View: to adjust view based on triggered event.
	///IEventTrigger* --> Notifications: to make sure the proper event triggered for the notification to display.
	///UserInputs --> UserInputTrigger: to specificy which user input event happened.
	///ObjectFactoryPresets --> for dynamic object generation
	std::vector<std::unique_ptr<Object>> notify(Event event, EventData data);
	Listner* getListner(Event event);
private:
	void addListner(Event event);
	void removeListner(Event event);
	
	std::map<Event, std::unique_ptr<Listner>> listners;

};
#endif // !EVENTHANDLER_H
