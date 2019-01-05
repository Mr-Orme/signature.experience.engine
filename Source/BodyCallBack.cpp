#include "BodyCallBack.h"
#include "ResourceManager.h"
#include "EventHandler.h"
#include "Listner.h"

BodyCallBack::BodyCallBack(
	ResourceManager* devices, 
	EventHandler::Event callBackEvent, 
	BodyComponent* owner, 
	EngineDefs::Vector adjustAmount, 
	void(BodyComponent::* BodyMethod)(EngineDefs::Vector) const, EventHandler::EventData secondaryData):
	owner(owner), adjustAmount(adjustAmount), BodyMethod(BodyMethod), secondaryData(secondaryData)
{
	devices->eventHandler->getListner(callBackEvent)->addCallBack(this);
}

void BodyCallBack::triggered(EventHandler::EventData data)
{
	if (data == secondaryData)
	{
		(owner->*BodyMethod)(adjustAmount);
	}
}
