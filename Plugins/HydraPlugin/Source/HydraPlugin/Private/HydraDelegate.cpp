#pragma once

#include "HydraPluginPrivatePCH.h"
#include "HydraDelegate.h"
#include "sixense.h"

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

				if (controller->trigger_pressed != previous->trigger_pressed)
				{
				
					if (controller->trigger_pressed)
					{
						EventHydraAnyButtonPressed(i);
						EventHydraTriggerPressed(i);
					}
					else{
						EventHydraTriggerReleased(i);
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
				}
				else{
					EventHydraBumperReleased(i);
				}
			}

			//B1
			if ((controller->buttons & SIXENSE_BUTTON_1) != (previous->buttons & SIXENSE_BUTTON_1))
			{
				if ((controller->buttons & SIXENSE_BUTTON_1) == SIXENSE_BUTTON_1)
				{
					EventHydraAnyButtonPressed(i);
					EventHydraB1Pressed(i);
				}
				else{
					EventHydraB1Released(i);
				}
			}
			//B2
			if ((controller->buttons & SIXENSE_BUTTON_2) != (previous->buttons & SIXENSE_BUTTON_2))
			{
				if ((controller->buttons & SIXENSE_BUTTON_2) == SIXENSE_BUTTON_2)
				{
					EventHydraAnyButtonPressed(i);
					EventHydraB2Pressed(i);
				}
				else{
					EventHydraB2Released(i);
				}
			}
			//B3
			if ((controller->buttons & SIXENSE_BUTTON_3) != (previous->buttons & SIXENSE_BUTTON_3))
			{
				if ((controller->buttons & SIXENSE_BUTTON_3) == SIXENSE_BUTTON_3)
				{
					EventHydraAnyButtonPressed(i);
					EventHydraB3Pressed(i);
				}
				else{
					EventHydraB3Released(i);
				}
			}
			//B4
			if ((controller->buttons & SIXENSE_BUTTON_4) != (previous->buttons & SIXENSE_BUTTON_4))
			{
				if ((controller->buttons & SIXENSE_BUTTON_4) == SIXENSE_BUTTON_4)
				{
					EventHydraAnyButtonPressed(i);
					EventHydraB4Pressed(i);
				}
				else{
					EventHydraB4Released(i);
				}
			}

			//Start
			if ((controller->buttons & SIXENSE_BUTTON_START) != (previous->buttons & SIXENSE_BUTTON_START))
			{
				if ((controller->buttons & SIXENSE_BUTTON_START) == SIXENSE_BUTTON_START)
				{
					EventHydraAnyButtonPressed(i);
					EventHydraStartPressed(i);
				}
				else{
					EventHydraStartReleased(i);
				}
			}

			//Joystick Click
			if ((controller->buttons & SIXENSE_BUTTON_JOYSTICK) != (previous->buttons & SIXENSE_BUTTON_JOYSTICK))
			{
				if ((controller->buttons & SIXENSE_BUTTON_JOYSTICK) == SIXENSE_BUTTON_JOYSTICK)
				{
					EventHydraAnyButtonPressed(i);
					EventHydraJoystickPressed(i);
				}
				else{
					EventHydraJoystickReleased(i);
				}
			}

			/** Movement */

			//Joystick
			if (controller->joystick.X != previous->joystick.X ||
				controller->joystick.Y != previous->joystick.Y)
			{
				EventHydraJoystickMoved(i, controller->joystick);
			}

			//Controller

			//Calculate Velocity and Acceleration
			controller->velocity = (controller->position - previous->position) / DeltaTime;
			controller->acceleration = (controller->velocity - previous->velocity) / DeltaTime;

			if (!controller->is_docked){

				//If the controller isn't docked, it's moving
				EventHydraControllerMoved(i,
					controller->position, controller->velocity, controller->acceleration,
					FRotator(controller->rotation));
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