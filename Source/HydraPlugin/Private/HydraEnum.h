#pragma once

#define MAX_CONTROLLERS_SUPPORTED 2
#define CONTROLLERS_PER_PLAYER	2

UENUM(BlueprintType)
enum EHydraControllerHand
{
	HYDRA_HAND_UNKNOWN,
	HYDRA_HAND_LEFT,
	HYDRA_HAND_RIGHT
};

UENUM(BlueprintType)
enum EHydraControllerButton
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

	static const FKey HydraLeftDocked;

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

	static const FKey HydraRightDocked;

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
	FVector rawPosition; //in cm without base offset
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
} SixenseControllerDataUE;

typedef struct _sixenseAllControllerDataUE{
	SixenseControllerDataUE controllers[4];		//current sdk max supported, untested on STEM or >2 controllers.
	int32 enabledCount;
	bool available;

	bool hasFullEnabledCount()
	{
		return enabledCount == MAX_CONTROLLERS_SUPPORTED;
	}
	bool isValidAndTracking()
	{
		return (available && hasFullEnabledCount());
	}
}SixenseAllControllerDataUE;