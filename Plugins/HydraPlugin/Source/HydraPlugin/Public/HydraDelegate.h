#pragma once
#include "CoreUObject.h"
#include "InputCore.h"

/** 
 * Converted Controller Data.
 * Contains converted raw and integrated 
 * sixense controller data.
 */
typedef struct _sixenseControllerDataUE{
	//raw converted
	FVector position;	//in hydra units (cm)
	FQuat rotation;		//raw, may need conversion
	FVector2D joystick;
	float trigger;
	uint32 buttons;
	uint8 sequence_number;
	uint16 firmware_revision;
	uint16 hardware_revision;
	uint16 packet_type;
	uint16 magnetic_frequency;
	bool enabled;
	int32 controller_index;
	bool is_docked;
	uint8 which_hand;
	bool hemi_tracking_enabled;

	//added values
	FVector velocity;
	FVector acceleration;
	bool trigger_pressed;
} sixenseControllerDataUE;

typedef struct _sixenseAllControllerDataUE{
	sixenseControllerDataUE controllers[4];		//current sdk max supported, untested on STEM or >2 controllers.
	int32 enabledCount;
	bool available;
}sixenseAllControllerDataUE;

struct EKeysHydra
{
	static const FKey HydraLeftJoystickX;
	static const FKey HydraLeftJoystickY;
	static const FKey HydraLeftJoystickClick;
	static const FKey HydraLeftB1;
	static const FKey HydraLeftB2;
	static const FKey HydraLeftB3;
	static const FKey HydraLeftB4;
	static const FKey HydraLeftStart;
	static const FKey HydraLeftTrigger;
	static const FKey HydraLeftTriggerClick;
	static const FKey HydraLeftBumper;

	//NB capped to 1.0 at 2 meters from base
	static const FKey HydraLeftMotionX;
	static const FKey HydraLeftMotionY;
	static const FKey HydraLeftMotionZ;

	static const FKey HydraLeftRotationPitch;
	static const FKey HydraLeftRotationYaw;
	static const FKey HydraLeftRotationRoll;

	static const FKey HydraRightJoystickX;
	static const FKey HydraRightJoystickY;
	static const FKey HydraRightJoystickClick;
	static const FKey HydraRightB1;
	static const FKey HydraRightB2;
	static const FKey HydraRightB3;
	static const FKey HydraRightB4;
	static const FKey HydraRightStart;
	static const FKey HydraRightTrigger;
	static const FKey HydraRightTriggerClick;
	static const FKey HydraRightBumper;

	//NB capped to 1.0 at 2 meters from base
	static const FKey HydraRightMotionX;
	static const FKey HydraRightMotionY;
	static const FKey HydraRightMotionZ;

	static const FKey HydraRightRotationPitch;
	static const FKey HydraRightRotationYaw;
	static const FKey HydraRightRotationRoll;
};

class HydraDelegate
{
	friend class FHydraPlugin;
public:
	//Namespace Hydra for variables and functions, EventHydra for events.

	/** Latest will always contain the freshest controller data */
	sixenseAllControllerDataUE* HydraLatestData;

	/** Holds last 10 controller captures, useful for gesture recognition*/
	sixenseAllControllerDataUE HydraHistoryData[10];

	/** Event Emitters, override to receive notifications.
	 *	int32 controller is the controller index (typically 0 or 1 for hydra) 
	 *	Call HydraWhichHand(controller index) to determine which hand is being held (determined and reset on docking)
	 */
	virtual void EventHydraPluggedIn();								//called once enabledCount == 2
	virtual void EventHydraUnplugged();								//called once enabledCount == 0
	virtual void EventHydraControllerEnabled(int32 controller);		//called for each controller
	virtual void EventHydraControllerDisabled(int32 controller);	//called for each controller

	virtual void EventHydraDocked(int32 controller);
	virtual void EventHydraUndocked(int32 controller);

	virtual void EventHydraAnyButtonPressed(int32 controller);
	virtual void EventHydraB1Pressed(int32 controller);
	virtual void EventHydraB1Released(int32 controller);
	virtual void EventHydraB2Pressed(int32 controller);
	virtual void EventHydraB2Released(int32 controller);
	virtual void EventHydraB3Pressed(int32 controller);
	virtual void EventHydraB3Released(int32 controller);
	virtual void EventHydraB4Pressed(int32 controller);
	virtual void EventHydraB4Released(int32 controller);
	virtual void EventHydraTriggerPressed(int32 controller);
	virtual void EventHydraTriggerReleased(int32 controller);
	virtual void EventHydraTriggerChanged(int32 controller, float value);	//Range 0-1.0
	virtual void EventHydraBumperPressed(int32 controller);
	virtual void EventHydraBumperReleased(int32 controller);
	virtual void EventHydraJoystickPressed(int32 controller);
	virtual void EventHydraJoystickReleased(int32 controller);
	virtual void EventHydraStartPressed(int32 controller);
	virtual void EventHydraStartReleased(int32 controller);

	virtual void EventHydraJoystickMoved(int32 controller, FVector2D movement);	//Range 0-1.0, 0-1.0
	
	/*	Movement in hydra units (should it be converted?)
	*	triggered whenever a controller is not docked. */
	virtual void EventHydraControllerMoved(int32 controller,						
								FVector position, FVector velocity, FVector acceleration, 
								FRotator rotation);

	//** Manual Check */
	virtual bool HydraIsAvailable();
	virtual int32 HydraWhichHand(int32 controller);	//call to determine which hand the controller is held in. Determined and reset on controller docking.

private:
	//Do not call, this will be called by the plugin, namespace InternalHydra
	void InternalHydraControllerTick(float DeltaTime);
	void InternalHydraUpdateAllData();
	void InternalHydraCheckEnabledCount(bool* plugNotChecked);
	void InternalHydraStartup();
};