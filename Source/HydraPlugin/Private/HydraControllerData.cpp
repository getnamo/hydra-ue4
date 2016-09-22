#include "HydraPluginPrivatePCH.h"
#include "HydraControllerData.h"
#include <sixense.h>

void FHydraControllerData::SetFromSixenseDataUE(const SixenseControllerDataUE& data)
{
	Position = data.position;
	Velocity = data.velocity;
	Acceleration = data.acceleration;

	RawPosition = data.rawPosition;
	
	Orientation = data.rotation;
	AngularVelocity = data.angular_velocity;

	Joystick = data.joystick;

	B1Pressed = (data.buttons & SIXENSE_BUTTON_1) == SIXENSE_BUTTON_1;
	B2Pressed = (data.buttons & SIXENSE_BUTTON_2) == SIXENSE_BUTTON_2;
	B3Pressed = (data.buttons & SIXENSE_BUTTON_3) == SIXENSE_BUTTON_3;
	B4Pressed = (data.buttons & SIXENSE_BUTTON_4) == SIXENSE_BUTTON_4;
	StartPressed = (data.buttons & SIXENSE_BUTTON_START) == SIXENSE_BUTTON_START;
	BumperPressed = (data.buttons & SIXENSE_BUTTON_BUMPER) == SIXENSE_BUTTON_BUMPER;
	JoystickClicked = (data.buttons & SIXENSE_BUTTON_JOYSTICK) == SIXENSE_BUTTON_JOYSTICK;
	TriggerPressed = data.trigger_pressed;

	Trigger = data.trigger;
	Docked = data.is_docked;
	Enabled = data.enabled;

	HandPossession = (EHydraControllerHand)data.which_hand;
}