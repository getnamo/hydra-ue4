#pragma once

#include "HydraDelegate.generated.h"

#define MAX_CONTROLLERS_SUPPORTED 2

UENUM(BlueprintType)
enum HydraControllerHand
{
	HYDRA_HAND_UNKNOWN,
	HYDRA_HAND_LEFT,
	HYDRA_HAND_RIGHT
};

UENUM(BlueprintType)
enum HydraControllerButton
{
	HYDRA_BUTTON_UNKNOWN,
	HYDRA_BUTTON_B1,
	HYDRA_BUTTON_B2,
	HYDRA_BUTTON_B3,
	HYDRA_BUTTON_B4,
	HYDRA_BUTTON_START,
	HYDRA_BUTTON_JOYSTICK,
	HYDRA_BUTTON_BUMPER,
	HYDRA_BUTTON_TRIGGER
};

//Input Mapping Keys
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

/** 
 * Converted Controller Data.
 * Contains converted raw and integrated 
 * sixense controller data.
 */
typedef struct _sixenseControllerDataUE{
	//raw converted
	FVector position;	//converted to cm
	FQuat quat;			//converted to ue space, rotation in quaternion format
	FRotator rotation;	//converted to ue space, use this version in blueprint
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
	FVector velocity;				//cm/s
	FVector acceleration;			//cm/s^2
	FRotator angular_velocity;		//deg/s
	bool trigger_pressed;
} sixenseControllerDataUE;

typedef struct _sixenseAllControllerDataUE{
	sixenseControllerDataUE controllers[4];		//current sdk max supported, untested on STEM or >2 controllers.
	int32 enabledCount;
	bool available;
}sixenseAllControllerDataUE;

class HydraDelegate
{
	friend class FHydraPlugin;
public:
	//Namespace Hydra for variables, functions and events.

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

	virtual void HydraButtonPressed(int32 controller, HydraControllerButton button);
	virtual void HydraButtonReleased(int32 controller, HydraControllerButton button);
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
	virtual HydraControllerHand HydraWhichHand(int32 controllerId);	//call to determine which hand the controller is held in. Determined and reset on controller docking.
	virtual sixenseControllerDataUE* HydraGetLatestData(int32 controllerId);
	virtual sixenseControllerDataUE* HydraGetHistoricalData(int32 controllerId, int32 historyIndex);

	//** Required Calls */
	virtual void HydraStartup();				//Call this somewhere in an initializing state such as BeginPlay()
	virtual void HydraTick(float DeltaTime);	//Call this every tick

	//Optional if you're extending data types, this is when you update the references, called from plugin
	virtual void UpdateControllerReference(sixenseControllerDataUE* controller, int index) {};
};