#pragma once

#include "HydraPluginPrivatePCH.h"
#include "HydraDelegate.h"
#include "sixense.h"
#include "Slate.h"

/** Empty Event Functions, no Super call required, because they don't do anything! */
void HydraDelegate::EventHydraControllerEnabled(int32 controller){}
void HydraDelegate::EventHydraControllerDisabled(int32 controller){}
void HydraDelegate::EventHydraPluggedIn(){}
void HydraDelegate::EventHydraUnplugged(){}
void HydraDelegate::EventHydraDocked(int32 controller){}
void HydraDelegate::EventHydraUndocked(int32 controller){}
void HydraDelegate::EventHydraAnyButtonPressed(int32 controller){}
void HydraDelegate::EventHydraB1Pressed(int32 controller){}
void HydraDelegate::EventHydraB1Released(int32 controller){}
void HydraDelegate::EventHydraB2Pressed(int32 controller){}
void HydraDelegate::EventHydraB2Released(int32 controller){}
void HydraDelegate::EventHydraB3Pressed(int32 controller){}
void HydraDelegate::EventHydraB3Released(int32 controller){}
void HydraDelegate::EventHydraB4Pressed(int32 controller){}
void HydraDelegate::EventHydraB4Released(int32 controller){}
void HydraDelegate::EventHydraTriggerPressed(int32 controller){}
void HydraDelegate::EventHydraTriggerReleased(int32 controller){}
void HydraDelegate::EventHydraTriggerChanged(int32 controller, float value){}
void HydraDelegate::EventHydraBumperPressed(int32 controller){}
void HydraDelegate::EventHydraBumperReleased(int32 controller){}
void HydraDelegate::EventHydraJoystickPressed(int32 controller){}
void HydraDelegate::EventHydraJoystickReleased(int32 controller){}
void HydraDelegate::EventHydraStartPressed(int32 controller){}
void HydraDelegate::EventHydraStartReleased(int32 controller){}

void HydraDelegate::EventHydraJoystickMoved(int32 controller, FVector2D movement){};
void HydraDelegate::EventHydraControllerMoved(int32 controller,
	FVector position, FVector velocity, FVector acceleration,
	FRotator rotation){};

/** Hydra Internal Functions, called by plugin.*/
void HydraDelegate::InternalHydraUpdateAllData()
{
	HydraHistoryData[9] = HydraHistoryData[8];
	HydraHistoryData[8] = HydraHistoryData[7];
	HydraHistoryData[7] = HydraHistoryData[6];
	HydraHistoryData[6] = HydraHistoryData[5];
	HydraHistoryData[5] = HydraHistoryData[4];
	HydraHistoryData[4] = HydraHistoryData[3];
	HydraHistoryData[3] = HydraHistoryData[2];
	HydraHistoryData[2] = HydraHistoryData[1];
	HydraHistoryData[1] = HydraHistoryData[0];
	//NB: HydraHistoryData[0] = *HydraLatestData gets updated after the tick to take in integrated data
}

void HydraDelegate::InternalHydraCheckEnabledCount(bool* plugNotChecked)
{
	if (!*plugNotChecked) return;

	sixenseAllControllerDataUE* previous = &HydraHistoryData[0];
	int32 oldCount = previous->enabledCount;
	int32 count = HydraLatestData->enabledCount;
	if (oldCount != count)
	{
		if (count == 2)	//hydra controller number, STEM behavior undefined.
		{
			EventHydraPluggedIn();
			*plugNotChecked = false;
		}
		else if (count == 0)
		{
			EventHydraUnplugged();
			*plugNotChecked = false;
		}
	}
}

//Function used for consistent conversion to input mapping basis
float MotionInputMappingConversion(float AxisValue){
	return FMath::Clamp(AxisValue / 200.f, -1.f, 1.f);
}

/** Internal Tick - Called by the Plugin */
void HydraDelegate::InternalHydraControllerTick(float DeltaTime)
{
	//Update Data History
	InternalHydraUpdateAllData();

	sixenseControllerDataUE* controller;
	sixenseControllerDataUE* previous;
	bool plugNotChecked = true;

	//Trigger any delegate events
	for (int i = 0; i < 4; i++)
	{
		controller = &HydraLatestData->controllers[i];
		previous = &HydraHistoryData[0].controllers[i];

		//If it is enabled run through all the event notifications and data integration
		if (controller->enabled)
		{
			//Enable Check
			if (controller->enabled != previous->enabled)
			{
				InternalHydraCheckEnabledCount(&plugNotChecked);
				EventHydraControllerEnabled(i);
			}

			//Docking
			if (controller->is_docked != previous->is_docked)
			{
				if (controller->is_docked)
				{
					EventHydraDocked(i);
				}
				else{
					EventHydraUndocked(i);
				}
			}

			//Determine Hand to support dynamic input mapping
			bool leftHand = HydraWhichHand(i) == 1;

			//** Buttons */

			//Trigger
			if (controller->trigger < 0.5)
			{
				controller->trigger_pressed = false;
			}
			else{
				controller->trigger_pressed = true;
			}

			if (controller->trigger != previous->trigger)
			{
				EventHydraTriggerChanged(i, controller->trigger);
				//InputMapping
				if (leftHand)
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraLeftTrigger, 0, controller->trigger);
				else
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraRightTrigger, 0, controller->trigger);

				if (controller->trigger_pressed != previous->trigger_pressed)
				{
				
					if (controller->trigger_pressed)
					{
						EventHydraAnyButtonPressed(i);
						EventHydraTriggerPressed(i);
						//InputMapping
						if (leftHand)
							FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraLeftTriggerClick, 0, 0);
						else
							FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraRightTriggerClick, 0, 0);
					}
					else{
						EventHydraTriggerReleased(i);
						//InputMapping
						if (leftHand)
							FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraLeftTriggerClick, 0, 0);
						else
							FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraRightTriggerClick, 0, 0);
					}
				}
			}

			//Bumper
			if ((controller->buttons & SIXENSE_BUTTON_BUMPER) != (previous->buttons & SIXENSE_BUTTON_BUMPER))
			{
				if ((controller->buttons & SIXENSE_BUTTON_BUMPER) == SIXENSE_BUTTON_BUMPER)
				{
					EventHydraAnyButtonPressed(i);
					EventHydraBumperPressed(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraLeftBumper, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraRightBumper, 0, 0);
				}
				else{
					EventHydraBumperReleased(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraLeftBumper, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraRightBumper, 0, 0);
				}
			}

			//B1
			if ((controller->buttons & SIXENSE_BUTTON_1) != (previous->buttons & SIXENSE_BUTTON_1))
			{
				if ((controller->buttons & SIXENSE_BUTTON_1) == SIXENSE_BUTTON_1)
				{
					EventHydraAnyButtonPressed(i);
					EventHydraB1Pressed(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraLeftB1, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraRightB1, 0, 0);
				}
				else{
					EventHydraB1Released(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraLeftB1, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraRightB1, 0, 0);
				}
			}
			//B2
			if ((controller->buttons & SIXENSE_BUTTON_2) != (previous->buttons & SIXENSE_BUTTON_2))
			{
				if ((controller->buttons & SIXENSE_BUTTON_2) == SIXENSE_BUTTON_2)
				{
					EventHydraAnyButtonPressed(i);
					EventHydraB2Pressed(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraLeftB2, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraRightB2, 0, 0);
				}
				else{
					EventHydraB2Released(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraLeftB2, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraRightB2, 0, 0);
				}
			}
			//B3
			if ((controller->buttons & SIXENSE_BUTTON_3) != (previous->buttons & SIXENSE_BUTTON_3))
			{
				if ((controller->buttons & SIXENSE_BUTTON_3) == SIXENSE_BUTTON_3)
				{
					EventHydraAnyButtonPressed(i);
					EventHydraB3Pressed(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraLeftB3, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraRightB3, 0, 0);
				}
				else{
					EventHydraB3Released(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraLeftB3, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraRightB3, 0, 0);
				}
			}
			//B4
			if ((controller->buttons & SIXENSE_BUTTON_4) != (previous->buttons & SIXENSE_BUTTON_4))
			{
				if ((controller->buttons & SIXENSE_BUTTON_4) == SIXENSE_BUTTON_4)
				{
					EventHydraAnyButtonPressed(i);
					EventHydraB4Pressed(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraLeftB4, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraRightB4, 0, 0);
				}
				else{
					EventHydraB4Released(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraLeftB4, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraRightB4, 0, 0);
				}
			}

			//Start
			if ((controller->buttons & SIXENSE_BUTTON_START) != (previous->buttons & SIXENSE_BUTTON_START))
			{
				if ((controller->buttons & SIXENSE_BUTTON_START) == SIXENSE_BUTTON_START)
				{
					EventHydraAnyButtonPressed(i);
					EventHydraStartPressed(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraLeftStart, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraRightStart, 0, 0);
				}
				else{
					EventHydraStartReleased(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraLeftStart, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraRightStart, 0, 0);
				}
			}

			//Joystick Click
			if ((controller->buttons & SIXENSE_BUTTON_JOYSTICK) != (previous->buttons & SIXENSE_BUTTON_JOYSTICK))
			{
				if ((controller->buttons & SIXENSE_BUTTON_JOYSTICK) == SIXENSE_BUTTON_JOYSTICK)
				{
					EventHydraAnyButtonPressed(i);
					EventHydraJoystickPressed(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraLeftJoystickClick, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraRightJoystickClick, 0, 0);
				}
				else{
					EventHydraJoystickReleased(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraLeftJoystickClick, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraRightJoystickClick, 0, 0);
				}
			}

			/** Movement */

			//Joystick
			if (controller->joystick.X != previous->joystick.X ||
				controller->joystick.Y != previous->joystick.Y)
			{
				EventHydraJoystickMoved(i, controller->joystick);
				//InputMapping
				if (leftHand)
				{
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraLeftJoystickX, 0, controller->joystick.X);
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraLeftJoystickY, 0, controller->joystick.Y);
				}
				else
				{
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraRightJoystickX, 0, controller->joystick.X);
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraRightJoystickY, 0, controller->joystick.Y);
				}
			}

			//Controller

			//Calculate Velocity and Acceleration
			controller->velocity = (controller->position - previous->position) / DeltaTime;
			controller->acceleration = (controller->velocity - previous->velocity) / DeltaTime;

			if (!controller->is_docked){
				FRotator rotation = FRotator(controller->rotation);

				//If the controller isn't docked, it's moving
				EventHydraControllerMoved(i,
					controller->position, controller->velocity, controller->acceleration,
					rotation);

				//InputMapping
				if (leftHand)
				{
					//2 meters = 1.0
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraLeftMotionX, 0, MotionInputMappingConversion(controller->position.X));
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraLeftMotionY, 0, MotionInputMappingConversion(controller->position.Y));
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraLeftMotionZ, 0, MotionInputMappingConversion(controller->position.Z));
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraLeftRotationPitch, 0, rotation.Pitch/90.f);
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraLeftRotationYaw, 0, rotation.Yaw/180.f);
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraLeftRotationRoll, 0, rotation.Roll/180.f);
				}
				else
				{
					//2 meters = 1.0
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraRightMotionX, 0, MotionInputMappingConversion(controller->position.X));
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraRightMotionY, 0, MotionInputMappingConversion(controller->position.Y));
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraRightMotionZ, 0, MotionInputMappingConversion(controller->position.Z));
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraRightRotationPitch, 0, rotation.Pitch/90.f);
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraRightRotationYaw, 0, rotation.Yaw/180.f);
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraRightRotationRoll, 0, rotation.Roll/180.f);
				}
			}
		}//end enabled
		else{
			if (controller->enabled != previous->enabled)
			{
				InternalHydraCheckEnabledCount(&plugNotChecked);
				EventHydraControllerDisabled(i);
			}
		}
	}//end controller for loop

	//Update the stored data with the integrated data obtained from latest
	HydraHistoryData[0] = *HydraLatestData;
}

void HydraDelegate::InternalHydraStartup()
{

}

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