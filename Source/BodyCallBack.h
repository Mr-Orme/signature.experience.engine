#ifndef BODY_CALLBACK_H
#define BODY_CALLBACK_H
#include "Definitions.h"
#include "ICallBack.h"
//#include "BodyComponent.h"
#include "InputDevice.h"
#include "EventHandler.h"
#include "ResourceManager.h"
class BodyComponent;
class BodyCallBack : public ICallBack
{
public:
	BodyCallBack(
		ResourceManager* devices,
		EventHandler::Event callBackEvent,
		BodyComponent* owner, 
		Vector2D adjustAmount, 
		void (BodyComponent::*BodyMethod)(Vector2D) const, 
		EventHandler::EventData secondaryData = 0);

	/*
	1. Keypressed
	2. idevice updated for that.
	3. userInputDevice checks iDevice for it's key-combo, if(true) trigger event, passing in
		memory address of first from key-combo map in iDevice.
	4. EventHandler notifies callBacks. 
	5. Body has callbacks with function pointer to proper BodyComponent function. 
		BodyCallBack also has EventData secondaryData that stores memory address of iDevice key-combo it is looking for
		calls proper body function based on passed in items secondaryData matches data passed in.
	???? Can I make this more generic? Probably, only if every compnent I want to use it for requires
		a Vector2D. Unless I can use templates to call the correct overload????
	*/
	void triggered(EventHandler::EventData data) override;
	//Pointer to method to call in BodyComponent
	void (BodyComponent::*BodyMethod)(Vector2D) const { nullptr };

	Vector2D adjustAmount{ 0.0f, 0.0f };
	BodyComponent* owner{ nullptr };
	EventHandler::EventData secondaryData{ 0 };


};
#endif // !CALLBACK_H
