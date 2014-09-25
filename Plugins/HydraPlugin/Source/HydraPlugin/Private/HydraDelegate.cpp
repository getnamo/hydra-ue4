#pragma once

#include "HydraPluginPrivatePCH.h"
#include "HydraDelegate.h"
#include "IHydraPlugin.h"

//FKey declarations
//Define each FKey const in a .cpp so we can compile
const FKey EKeysHydra::HydraLeftJoystickX("HydraLeftJoystickX");
const FKey EKeysHydra::HydraLeftJoystickY("HydraLeftJoystickY");
const FKey EKeysHydra::HydraLeftJoystickClick("HydraLeftJoystickClick");
const FKey EKeysHydra::HydraLeftB1("HydraLeftB1");
const FKey EKeysHydra::HydraLeftB2("HydraLeftB2");
const FKey EKeysHydra::HydraLeftB3("HydraLeftB3");
const FKey EKeysHydra::HydraLeftB4("HydraLeftB4");
const FKey EKeysHydra::HydraLeftStart("HydraLeftStart");
const FKey EKeysHydra::HydraLeftTrigger("HydraLeftTrigger");
const FKey EKeysHydra::HydraLeftTriggerClick("HydraLeftTriggerAxis");
const FKey EKeysHydra::HydraLeftBumper("HydraLeftBumper");

const FKey EKeysHydra::HydraLeftMotionX("HydraLeftMotionX");
const FKey EKeysHydra::HydraLeftMotionY("HydraLeftMotionY");
const FKey EKeysHydra::HydraLeftMotionZ("HydraLeftMotionZ");

const FKey EKeysHydra::HydraLeftRotationPitch("HydraLeftRotationPitch");
const FKey EKeysHydra::HydraLeftRotationYaw("HydraLeftRotationYaw");
const FKey EKeysHydra::HydraLeftRotationRoll("HydraLeftRotationRoll");

const FKey EKeysHydra::HydraRightJoystickX("HydraRightJoystickX");
const FKey EKeysHydra::HydraRightJoystickY("HydraRightJoystickY");
const FKey EKeysHydra::HydraRightJoystickClick("HydraRightJoystickClick");
const FKey EKeysHydra::HydraRightB1("HydraRightB1");
const FKey EKeysHydra::HydraRightB2("HydraRightB2");
const FKey EKeysHydra::HydraRightB3("HydraRightB3");
const FKey EKeysHydra::HydraRightB4("HydraRightB4");
const FKey EKeysHydra::HydraRightStart("HydraRightStart");
const FKey EKeysHydra::HydraRightTrigger("HydraRightTrigger");
const FKey EKeysHydra::HydraRightTriggerClick("HydraRightTriggerAxis");
const FKey EKeysHydra::HydraRightBumper("HydraRightBumper");

const FKey EKeysHydra::HydraRightMotionX("HydraRightMotionX");
const FKey EKeysHydra::HydraRightMotionY("HydraRightMotionY");
const FKey EKeysHydra::HydraRightMotionZ("HydraRightMotionZ");

const FKey EKeysHydra::HydraRightRotationPitch("HydraRightRotationPitch");
const FKey EKeysHydra::HydraRightRotationYaw("HydraRightRotationYaw");
const FKey EKeysHydra::HydraRightRotationRoll("HydraRightRotationRoll");

/** Empty Event Functions, no Super call required, because they don't do anything! */
void HydraDelegate::HydraControllerEnabled(int32 controller){}
void HydraDelegate::HydraControllerDisabled(int32 controller){}
void HydraDelegate::HydraPluggedIn(){}
void HydraDelegate::HydraUnplugged(){}
void HydraDelegate::HydraDocked(int32 controller){}
void HydraDelegate::HydraUndocked(int32 controller){}
void HydraDelegate::HydraAnyButtonPressed(int32 controller){}
void HydraDelegate::HydraB1Pressed(int32 controller){}
void HydraDelegate::HydraB1Released(int32 controller){}
void HydraDelegate::HydraB2Pressed(int32 controller){}
void HydraDelegate::HydraB2Released(int32 controller){}
void HydraDelegate::HydraB3Pressed(int32 controller){}
void HydraDelegate::HydraB3Released(int32 controller){}
void HydraDelegate::HydraB4Pressed(int32 controller){}
void HydraDelegate::HydraB4Released(int32 controller){}
void HydraDelegate::HydraTriggerPressed(int32 controller){}
void HydraDelegate::HydraTriggerReleased(int32 controller){}
void HydraDelegate::HydraTriggerChanged(int32 controller, float value){}
void HydraDelegate::HydraBumperPressed(int32 controller){}
void HydraDelegate::HydraBumperReleased(int32 controller){}
void HydraDelegate::HydraJoystickPressed(int32 controller){}
void HydraDelegate::HydraJoystickReleased(int32 controller){}
void HydraDelegate::HydraStartPressed(int32 controller){}
void HydraDelegate::HydraStartReleased(int32 controller){}

void HydraDelegate::HydraJoystickMoved(int32 controller, FVector2D movement){};
void HydraDelegate::HydraControllerMoved(int32 controller,
	FVector position, FVector velocity, FVector acceleration,
	FRotator rotation, FRotator angularVelocity){};

/** Availability */
bool HydraDelegate::HydraIsAvailable()
{
	//hydra will always have an enabled count of 2 when plugged in and working, stem functionality undefined.
	return HydraLatestData->enabledCount == 2;
}

/** Call to determine which hand you're holding the controller in. Determine by last docking position.*/
int32 HydraDelegate::HydraWhichHand(int32 controller)
{
	return HydraLatestData->controllers[controller].which_hand;
}

/** Poll for latest data.*/
bool HydraDelegate::HydraGetLatestData(int32 controller, FVector& position, FVector& velocity, FVector& acceleration, FRotator& rotation, FRotator& angularVelocity,
	FVector2D& joystick, int32& buttons, float& trigger, bool& docked)
{
	if (controller > 4 || controller < 0){ return false; }

	return HydraGetHistoricalData(controller, 0, position, velocity, acceleration, rotation, angularVelocity ,joystick, buttons, trigger, docked);
}

/** Poll for historical data. Valid index is 0-9 */
bool HydraDelegate::HydraGetHistoricalData(int32 controller, int32 historyIndex, FVector& position, FVector& velocity, FVector& acceleration, FRotator& rotation, FRotator& angularVelocity,
	FVector2D& joystick, int32& buttons, float& trigger, bool& docked)
{
	if (historyIndex<0 || historyIndex>9){ return false; }
	if (controller > 4 || controller < 0){ return false; }

	sixenseControllerDataUE* data = &HydraHistoryData[historyIndex].controllers[controller];

	position = data->position;
	velocity = data->velocity;
	acceleration = data->acceleration;
	rotation = data->rotation;
	angularVelocity = data->angular_velocity;
	joystick = data->joystick;
	buttons = data->buttons;
	trigger = data->trigger;
	docked = data->is_docked;
	return data->enabled;
}

void HydraDelegate::HydraStartup()
{
	if (IHydraPlugin::IsAvailable())
	{
		//Required to Work - Set self as a delegate
		IHydraPlugin::Get().SetDelegate((HydraDelegate*)this);
	}
}
void HydraDelegate::HydraTick(float DeltaTime)
{
	if (IHydraPlugin::IsAvailable())
	{
		//Required to Work - This is the plugin magic
		IHydraPlugin::Get().HydraTick(DeltaTime);
	}
}
