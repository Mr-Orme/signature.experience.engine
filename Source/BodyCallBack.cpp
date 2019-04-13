#include "BodyCallBack.h"
#include "ResourceManager.h"
#include "EventHandler.h"
#include "Listner.h"
#include "Vector2D.h"

BodyCallBack::BodyCallBack(
	ResourceManager* devices, 
	EventHandler::Event callBackEvent, 
	BodyComponent* owner, 
	Vector2D adjustAmount, 
	void(BodyComponent::* BodyMethod)(Vector2D) const, EventHandler::EventData secondaryData):
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
