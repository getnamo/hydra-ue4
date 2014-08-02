#pragma once

#include "HydraPluginPrivatePCH.h"
#include "HydraDelegate.h"
#include "IHydraPlugin.h"

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
