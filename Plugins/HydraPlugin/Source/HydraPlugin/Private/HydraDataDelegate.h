#pragma once

#include "HydraEnum.h"
#include "IMotionController.h"

class UHydraSingleController;
class UHydraPluginComponent;

class HydraDataDelegate
{
	friend class FHydraController;
public:
	HydraDataDelegate();
	~HydraDataDelegate();
	
	//Namespace Hydra for variables, functions and events.


	//Controller short-hands
	UHydraSingleController* LeftController = nullptr;
	int32 LeftControllerId = 0;
	UHydraSingleController* RightController = nullptr;
	int32 RightControllerId = 1;

	//Offset to base
	FVector baseOffset = FVector(0, 0, 0);
	
	//Event multi-cast delegation
	TArray<UHydraPluginComponent*> eventDelegates;
	void AddEventDelegate(UHydraPluginComponent* delegate);
	void RemoveEventDelegate(UHydraPluginComponent* delegate);

	void CallFunctionOnDelegates(TFunction< void(UHydraPluginComponent*)> InFunction);
	
	/** Latest will always contain the freshest controller data, external pointer do not delete*/
	sixenseAllControllerDataUE* HydraLatestData;

	/** Holds last 10 controller captures, useful for gesture recognition, external pointer do not delete*/
	sixenseAllControllerDataUE* HydraHistoryData;	//dynamic array size 10

	/** Event Emitters, override to receive notifications.
	 *	int32 controller is the controller index (typically 0 or 1 for hydra) 
	 *	Call HydraWhichHand(controller index) to determine which hand is being held (determined and reset on docking)
	 */
	virtual void HydraPluggedIn();								//called once enabledCount == 2
	virtual void HydraUnplugged();								//called once enabledCount == 0
	virtual void HydraControllerEnabled(int32 controller);		//called for each controller
	virtual void HydraControllerDisabled(int32 controller);	//called for each controller

	virtual void HydraDocked(int32 controller);
	virtual void HydraUndocked(int32 controller);

	virtual void HydraButtonPressed(int32 controller, EHydraControllerButton button);
	virtual void HydraButtonReleased(int32 controller, EHydraControllerButton button);
	virtual void HydraB1Pressed(int32 controller);
	virtual void HydraB1Released(int32 controller);
	virtual void HydraB2Pressed(int32 controller);
	virtual void HydraB2Released(int32 controller);
	virtual void HydraB3Pressed(int32 controller);
	virtual void HydraB3Released(int32 controller);
	virtual void HydraB4Pressed(int32 controller);
	virtual void HydraB4Released(int32 controller);
	virtual void HydraTriggerPressed(int32 controller);
	virtual void HydraTriggerReleased(int32 controller);
	virtual void HydraTriggerChanged(int32 controller, float value);	//Range 0-1.0
	virtual void HydraBumperPressed(int32 controller);
	virtual void HydraBumperReleased(int32 controller);
	virtual void HydraJoystickPressed(int32 controller);
	virtual void HydraJoystickReleased(int32 controller);
	virtual void HydraStartPressed(int32 controller);
	virtual void HydraStartReleased(int32 controller);

	virtual void HydraJoystickMoved(int32 controller, FVector2D movement);	//Range 0-1.0, 0-1.0
	
	/*	Movement converted into ue space (cm units in x,y,z and degrees in rotation) */
	virtual void HydraControllerMoved(int32 controller,						
								FVector position, FVector velocity, FVector acceleration, 
								FRotator rotation, FRotator angularVelocity);

	//** Callable Functions (for public polling support) */
	virtual bool HydraIsAvailable();
	virtual EHydraControllerHand HydraWhichHand(int32 controllerId);	//call to determine which hand the controller is held in. Determined and reset on controller docking.
	virtual UHydraSingleController* HydraControllerForID(int32 controllerId);
	virtual UHydraSingleController* HydraControllerForControllerHand(EControllerHand hand);
	virtual UHydraSingleController* HydraControllerForHydraHand(EHydraControllerHand hand);
	virtual sixenseControllerDataUE* HydraGetLatestData(int32 controllerId);
	virtual sixenseControllerDataUE* HydraGetHistoricalData(int32 controllerId, int32 historyIndex);

	void UpdateControllerReference(sixenseControllerDataUE* controller, int index);
};