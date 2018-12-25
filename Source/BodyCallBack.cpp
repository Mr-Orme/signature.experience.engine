#include "BodyCallBack.h"

BodyCallBack::BodyCallBack(
	ResourceManager* devices, EventHandler::Event callBackEvent, BodyComponent* owner, EngineDefs::Vector adjustAmount, void(BodyComponent::* BodyCall)(EngineDefs::Vector) const, void * secondaryData):
	owner(owner), adjustAmount(adjustAmount), BodyCall(BodyCall), secondaryData(secondaryData)
{
	devices->eventHandler->getListner(callBackEvent)->addCallBack(this);
}

void BodyCallBack::triggered(void * data)
{
	if (data == secondaryData)
	{
		(owner->*BodyCall)(adjustAmount);
	}
}
