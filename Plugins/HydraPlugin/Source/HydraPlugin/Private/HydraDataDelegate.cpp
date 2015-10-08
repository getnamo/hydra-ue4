#pragma once

#include "HydraPluginPrivatePCH.h"
#include "HydraDataDelegate.h"
#include "HydraSingleController.h"
#include "HydraComponent.h"

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

const FKey EKeysHydra::HydraLeftDocked("HydraLeftDocked");

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

const FKey EKeysHydra::HydraRightDocked("HydraRightDocked");

const FKey EKeysHydra::HydraRightMotionX("HydraRightMotionX");
const FKey EKeysHydra::HydraRightMotionY("HydraRightMotionY");
const FKey EKeysHydra::HydraRightMotionZ("HydraRightMotionZ");

const FKey EKeysHydra::HydraRightRotationPitch("HydraRightRotationPitch");
const FKey EKeysHydra::HydraRightRotationYaw("HydraRightRotationYaw");
const FKey EKeysHydra::HydraRightRotationRoll("HydraRightRotationRoll");

/** Empty Event Functions, no Super call required, because they don't do anything. Kept for easy future extension */
void HydraDataDelegate::HydraControllerEnabled(int32 controller){}
void HydraDataDelegate::HydraControllerDisabled(int32 controller){}

//Except the ones that are hooked into multi-cast delegates, which call each attached delegate with the given function via a lambda pass
void HydraDataDelegate::HydraPluggedIn()
{
	CallFunctionOnDelegates([](UHydraPluginComponent* delegate)
	{
		delegate->PluggedIn.Broadcast();
	});
}
void HydraDataDelegate::HydraUnplugged()
{
	CallFunctionOnDelegates([](UHydraPluginComponent* delegate)
	{
		delegate->Unplugged.Broadcast();
	});
}
void HydraDataDelegate::HydraDocked(int32 controllerId)
{
	UHydraSingleController* controller = HydraControllerForID(controllerId);
	CallFunctionOnDelegates([&](UHydraPluginComponent* delegate)
	{
		//Specialized function handles the broadcast
		delegate->Docked(controller);
	});
}
void HydraDataDelegate::HydraUndocked(int32 controllerId)
{
	UHydraSingleController* controller = HydraControllerForID(controllerId);
	CallFunctionOnDelegates([&](UHydraPluginComponent* delegate)
	{
		//Specialized function handles the broadcast
		delegate->Undocked(controller);
	});
}

void HydraDataDelegate::HydraButtonPressed(int32 controllerId, EHydraControllerButton button)
{
	UHydraSingleController* controller = HydraControllerForID(controllerId);
	CallFunctionOnDelegates([&](UHydraPluginComponent* delegate)
	{
		delegate->ButtonPressed.Broadcast(controller, button);
	});
}
void HydraDataDelegate::HydraButtonReleased(int32 controllerId, EHydraControllerButton button)
{
	UHydraSingleController* controller = HydraControllerForID(controllerId);
	CallFunctionOnDelegates([&](UHydraPluginComponent* delegate)
	{
		delegate->ButtonReleased.Broadcast(controller, button);
	});
}
void HydraDataDelegate::HydraB1Pressed(int32 controllerId){}
void HydraDataDelegate::HydraB1Released(int32 controllerId){}
void HydraDataDelegate::HydraB2Pressed(int32 controllerId){}
void HydraDataDelegate::HydraB2Released(int32 controllerId){}
void HydraDataDelegate::HydraB3Pressed(int32 controllerId){}
void HydraDataDelegate::HydraB3Released(int32 controllerId){}
void HydraDataDelegate::HydraB4Pressed(int32 controllerId){}
void HydraDataDelegate::HydraB4Released(int32 controllerId){}
void HydraDataDelegate::HydraTriggerPressed(int32 controllerId){}
void HydraDataDelegate::HydraTriggerReleased(int32 controllerId){}
void HydraDataDelegate::HydraTriggerChanged(int32 controllerId, float value){}
void HydraDataDelegate::HydraBumperPressed(int32 controllerId){}
void HydraDataDelegate::HydraBumperReleased(int32 controllerId){}
void HydraDataDelegate::HydraJoystickPressed(int32 controllerId){}
void HydraDataDelegate::HydraJoystickReleased(int32 controllerId){}
void HydraDataDelegate::HydraStartPressed(int32 controllerId){}
void HydraDataDelegate::HydraStartReleased(int32 controllerId){}

void HydraDataDelegate::HydraJoystickMoved(int32 controllerId, FVector2D movement)
{
	UHydraSingleController* controller = HydraControllerForID(controllerId);
	CallFunctionOnDelegates([&](UHydraPluginComponent* delegate)
	{
		delegate->JoystickMoved.Broadcast(controller, movement);
	});
};
void HydraDataDelegate::HydraControllerMoved(int32 controllerId,
	FVector position, FVector velocity, FVector acceleration,
	FRotator rotation, FRotator angularVelocity)
{
	UHydraSingleController* controller = HydraControllerForID(controllerId);
	CallFunctionOnDelegates([&](UHydraPluginComponent* delegate)
	{
		delegate->ControllerMoved.Broadcast(controller,position,velocity,acceleration,rotation,angularVelocity);
	});
};

/** Availability */
bool HydraDataDelegate::HydraIsAvailable()
{
	//hydra will always have an enabled count of 2 when plugged in and working, stem functionality undefined.
	return HydraLatestData->enabledCount == 2;
}


void HydraDataDelegate::AddEventDelegate(UHydraPluginComponent* delegate)
{
	eventDelegates.Add(delegate);
}

void HydraDataDelegate::RemoveEventDelegate(UHydraPluginComponent* delegate)
{
	eventDelegates.Remove(delegate);
}


void HydraDataDelegate::CallFunctionOnDelegates(TFunction< void(UHydraPluginComponent*)> InFunction)
{
	for (UHydraPluginComponent* eventDelegate : eventDelegates)
	{
		InFunction(eventDelegate);
	}
}

HydraDataDelegate::HydraDataDelegate()
{
	//Ugly Init, but ensures no instance leakage, if you know how to remove the root references and attach this to a plugin parent UObject, make a pull request!
	LeftController = NewObject<UHydraSingleController>();
	LeftController->AddToRoot();	//we root these otherwise they can be removed early
	RightController = NewObject<UHydraSingleController>();
	RightController->AddToRoot();
}
HydraDataDelegate::~HydraDataDelegate(){
	//Cleanup root references
	if (LeftController)
		LeftController->RemoveFromRoot();
	if (RightController)
		RightController->RemoveFromRoot();
}

/** Call to determine which hand you're holding the controller in. Determine by last docking position.*/
EHydraControllerHand HydraDataDelegate::HydraWhichHand(int32 controller)
{
	return (EHydraControllerHand)HydraLatestData->controllers[controller].which_hand;
}


UHydraSingleController* HydraDataDelegate::HydraControllerForID(int32 controllerId)
{
	return HydraControllerForHydraHand(HydraWhichHand(controllerId));
}

UHydraSingleController* HydraDataDelegate::HydraControllerForControllerHand(EControllerHand hand)
{
	switch (hand)
	{
	case EControllerHand::Left:
		return LeftController;
		break;
	case EControllerHand::Right:
		return RightController;
		break;
	}
	return nullptr;
}

UHydraSingleController* HydraDataDelegate::HydraControllerForHydraHand(EHydraControllerHand hand)
{
	switch (hand){
	case HYDRA_HAND_LEFT:
		return LeftController;
		break;
	case HYDRA_HAND_RIGHT:
		return RightController;
		break;
	case HYDRA_HAND_UNKNOWN:
		return nullptr;
		break;
	}
	return nullptr;
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
	EHydraControllerHand hand = HydraWhichHand(index);

	if (hand == HYDRA_HAND_LEFT)
	{
		LeftController->setFromSixenseDataUE(controller);
		LeftControllerId = index;
	}
	else if (hand == HYDRA_HAND_RIGHT)
	{
		RightController->setFromSixenseDataUE(controller);
		RightControllerId = index;
	}
}

