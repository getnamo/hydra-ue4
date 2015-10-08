#include "HydraPluginPrivatePCH.h"
#include "HydraSingleController.h"
#include <sixense.h>

UHydraSingleController::UHydraSingleController(const FObjectInitializer &init) : UObject(init)
{
}

bool UHydraSingleController::isLeftHand()
{
	return handPossession == EHydraControllerHand::HYDRA_HAND_LEFT;
}

bool UHydraSingleController::isRightHand()
{
	return handPossession == EHydraControllerHand::HYDRA_HAND_RIGHT;
}

void UHydraSingleController::setFromSixenseDataUE(sixenseControllerDataUE* data)
{
	this->position = data->position;
	this->rawPosition = data->rawPosition;
	this->velocity = data->velocity;
	this->acceleration = data->acceleration;
	
	this->orientation = data->rotation;
	this->angularVelocity = data->angular_velocity;

	this->joystick = data->joystick;

	this->b1Pressed = (data->buttons & SIXENSE_BUTTON_1) == SIXENSE_BUTTON_1;
	this->b2Pressed = (data->buttons & SIXENSE_BUTTON_2) == SIXENSE_BUTTON_2;
	this->b3Pressed = (data->buttons & SIXENSE_BUTTON_3) == SIXENSE_BUTTON_3;
	this->b4Pressed = (data->buttons & SIXENSE_BUTTON_4) == SIXENSE_BUTTON_4;
	this->startPressed = (data->buttons & SIXENSE_BUTTON_START) == SIXENSE_BUTTON_START;
	this->bumperPressed = (data->buttons & SIXENSE_BUTTON_BUMPER) == SIXENSE_BUTTON_BUMPER;
	this->joystickClicked = (data->buttons & SIXENSE_BUTTON_JOYSTICK) == SIXENSE_BUTTON_JOYSTICK;
	this->triggerPressed = data->trigger_pressed;

	this->trigger = data->trigger;
	this->docked = data->is_docked;

	this->handPossession = (EHydraControllerHand)data->which_hand;
}