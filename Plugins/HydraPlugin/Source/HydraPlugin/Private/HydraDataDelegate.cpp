#pragma once

#include "HydraPluginPrivatePCH.h"
#include "HydraDataDelegate.h"
#include "HydraSingleController.h"

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
void HydraDataDelegate::HydraControllerEnabled(int32 controller){}
void HydraDataDelegate::HydraControllerDisabled(int32 controller){}
void HydraDataDelegate::HydraPluggedIn(){}
void HydraDataDelegate::HydraUnplugged(){}
void HydraDataDelegate::HydraDocked(int32 controller){}
void HydraDataDelegate::HydraUndocked(int32 controller){}
void HydraDataDelegate::HydraButtonPressed(int32 controller, HydraControllerButton button){}
void HydraDataDelegate::HydraButtonReleased(int32 controller, HydraControllerButton button){}
void HydraDataDelegate::HydraB1Pressed(int32 controller){}
void HydraDataDelegate::HydraB1Released(int32 controller){}
void HydraDataDelegate::HydraB2Pressed(int32 controller){}
void HydraDataDelegate::HydraB2Released(int32 controller){}
void HydraDataDelegate::HydraB3Pressed(int32 controller){}
void HydraDataDelegate::HydraB3Released(int32 controller){}
void HydraDataDelegate::HydraB4Pressed(int32 controller){}
void HydraDataDelegate::HydraB4Released(int32 controller){}
void HydraDataDelegate::HydraTriggerPressed(int32 controller){}
void HydraDataDelegate::HydraTriggerReleased(int32 controller){}
void HydraDataDelegate::HydraTriggerChanged(int32 controller, float value){}
void HydraDataDelegate::HydraBumperPressed(int32 controller){}
void HydraDataDelegate::HydraBumperReleased(int32 controller){}
void HydraDataDelegate::HydraJoystickPressed(int32 controller){}
void HydraDataDelegate::HydraJoystickReleased(int32 controller){}
void HydraDataDelegate::HydraStartPressed(int32 controller){}
void HydraDataDelegate::HydraStartReleased(int32 controller){}

void HydraDataDelegate::HydraJoystickMoved(int32 controller, FVector2D movement){};
void HydraDataDelegate::HydraControllerMoved(int32 controller,
	FVector position, FVector velocity, FVector acceleration,
	FRotator rotation, FRotator angularVelocity){};

/** Availability */
bool HydraDataDelegate::HydraIsAvailable()
{
	//hydra will always have an enabled count of 2 when plugged in and working, stem functionality undefined.
	return HydraLatestData->enabledCount == 2;
}

/** Call to determine which hand you're holding the controller in. Determine by last docking position.*/
HydraControllerHand HydraDataDelegate::HydraWhichHand(int32 controller)
{
	return (HydraControllerHand)HydraLatestData->controllers[controller].which_hand;
}

/** Poll for latest data.*/

sixenseControllerDataUE* HydraDataDelegate::HydraGetLatestData(int32 controllerId)
{
	if ((controllerId > MAX_CONTROLLERS_SUPPORTED) || (controllerId < 0))
	{
		return NULL; 
	}
	return HydraGetHistoricalData(controllerId, 0);
}

/** Poll for historical data. Valid index is 0-9 */
sixenseControllerDataUE* HydraDataDelegate::HydraGetHistoricalData(int32 controllerId, int32 historyIndex)
{
	if ((historyIndex<0) || (historyIndex>9) || (controllerId > MAX_CONTROLLERS_SUPPORTED) || (controllerId < 0))
	{
		return NULL;
	}
	sixenseControllerDataUE* data;
	data = &HydraHistoryData[historyIndex].controllers[controllerId];

	return data;
}

void HydraDataDelegate::UpdateControllerReference(sixenseControllerDataUE* controller, int index)
{
	//Get the hand index and set
	HydraControllerHand hand = HydraWhichHand(index);

	if (hand == HYDRA_HAND_LEFT)
	{
		if (LeftController== nullptr)
		{
			LeftController = NewObject<UHydraSingleController>(UHydraSingleController::StaticClass());
		}
		LeftController->setFromSixenseDataUE(controller);
	}
	else if (hand == HYDRA_HAND_RIGHT)
	{
		if (RightController == nullptr)
		{
			RightController = NewObject<UHydraSingleController>(UHydraSingleController::StaticClass());
		}
		RightController->setFromSixenseDataUE(controller);
	}
}
