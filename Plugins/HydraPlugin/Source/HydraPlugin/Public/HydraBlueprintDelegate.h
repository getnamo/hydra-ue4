#pragma once

#include "HydraDelegate.h"
#include "HydraInterface.h"
#include "Core.h"
//#include "HydraBlueprintDelegate.generated.h"


class HydraBlueprintDelegate : public HydraDelegate
{
public:
	
	//Events which are forwarded to the interface class. NB: not forwarded to component bp
	void HydraPluggedIn() override;
	void HydraUnplugged() override;
	void HydraControllerEnabled(int32 controller) override;
	void HydraControllerDisabled(int32 controller) override;
	void HydraDocked(int32 controller) override;
	void HydraUndocked(int32 controller) override;
	void HydraButtonPressed(int32 controller, HydraControllerButton button) override;
	void HydraButtonReleased(int32 controller, HydraControllerButton button) override;
	void HydraB1Pressed(int32 controller) override;
	void HydraB1Released(int32 controller) override;
	void HydraB2Pressed(int32 controller) override;
	void HydraB2Released(int32 controller) override;
	void HydraB3Pressed(int32 controller) override;
	void HydraB3Released(int32 controller) override;
	void HydraB4Pressed(int32 controller) override;
	void HydraB4Released(int32 controller) override;
	void HydraTriggerPressed(int32 controller) override;
	void HydraTriggerReleased(int32 controller) override;
	void HydraTriggerChanged(int32 controller, float value) override;
	void HydraBumperPressed(int32 controller) override;
	void HydraBumperReleased(int32 controller) override;
	void HydraJoystickPressed(int32 controller) override;
	void HydraJoystickReleased(int32 controller) override;
	void HydraStartPressed(int32 controller) override;
	void HydraStartReleased(int32 controller) override;
	void HydraJoystickMoved(int32 controller, FVector2D movement) override;
	void HydraControllerMoved(int32 controller,
		FVector position, FVector velocity, FVector acceleration,
		FRotator rotation, FRotator angularVelocity) override;

	//Callable Blueprint functions
	/** Check if the hydra is available/plugged in.*/
	bool HydraIsAvailable();

	/** Poll for historical data. Valid ControllerId is 0 or 1, Valid history index is 0-9.*/
	UHydraSingleController* HydraGetHistoricalFrames(int32 controllerId, int32 historyIndex);

	//** Get the latest available data given in a single frame. Valid ControllerId is 0 or 1  */
	UHydraSingleController* HydraGetLatestFrame(int32 controllerId);

	//** Required Calls */
	virtual void HydraStartup() override;				//Call this somewhere in an initializing state such as BeginPlay()
	virtual void HydraTick(float DeltaTime) override;	//Call this every tick

	void UpdateControllerReference(sixenseControllerDataUE* controller, int index) override;

	//If you want an alternate delegate, set it here
	void SetInterfaceDelegate(UObject* newDelegate);

protected:
	UObject* ValidSelfPointer;	//REQUIRED: has to be set before HydraStartup by a UObject subclass.
private:
	class UHydraSingleController* _latestFrame[MAX_CONTROLLERS_SUPPORTED];
	UObject* _interfaceDelegate;
	bool implementsInterface();
	UHydraSingleController* controllerForId(int controllerId);
};