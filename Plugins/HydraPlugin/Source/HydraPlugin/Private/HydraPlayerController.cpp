#include "HydraPluginPrivatePCH.h"

#include "IHydraPlugin.h"
#include "HydraDelegate.h"
#include "HydraPlayerController.h"

//Constructor/Initializer
AHydraPlayerController::AHydraPlayerController(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
}

//Override implementation
void AHydraPlayerController::EventHydraControllerEnabled(int32 controller)
{
	HydraControllerEnabled(controller);
}
void AHydraPlayerController::EventHydraControllerDisabled(int32 controller)
{
	HydraControllerDisabled(controller);
}
void AHydraPlayerController::EventHydraPluggedIn()
{
	HydraPluggedIn();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Hydra Plugged in.")));
}
void AHydraPlayerController::EventHydraUnplugged()
{
	HydraUnplugged();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Hydra Unplugged.")));
}
void AHydraPlayerController::EventHydraDocked(int32 controller)
{
	HydraDocked(controller);
}
void AHydraPlayerController::EventHydraUndocked(int32 controller)
{
	HydraUndocked(controller);
}
void AHydraPlayerController::EventHydraAnyButtonPressed(int32 controller)
{
	HydraAnyButtonPressed(controller);
}
void AHydraPlayerController::EventHydraB1Pressed(int32 controller)
{
	HydraB1Pressed(controller);
}
void AHydraPlayerController::EventHydraB1Released(int32 controller)
{
	HydraB1Released(controller);
}
void AHydraPlayerController::EventHydraB2Pressed(int32 controller)
{
	HydraB2Pressed(controller);
}
void AHydraPlayerController::EventHydraB2Released(int32 controller)
{
	HydraB2Released(controller);
}
void AHydraPlayerController::EventHydraB3Pressed(int32 controller)
{
	HydraB3Pressed(controller);
}
void AHydraPlayerController::EventHydraB3Released(int32 controller)
{
	HydraB3Released(controller);
}
void AHydraPlayerController::EventHydraB4Pressed(int32 controller)
{
	HydraB4Pressed(controller);
}
void AHydraPlayerController::EventHydraB4Released(int32 controller)
{
	HydraB4Released(controller);
}
void AHydraPlayerController::EventHydraTriggerPressed(int32 controller)
{
	HydraTriggerPressed(controller);
}
void AHydraPlayerController::EventHydraTriggerReleased(int32 controller)
{
	HydraTriggerReleased(controller);
}
void AHydraPlayerController::EventHydraTriggerChanged(int32 controller, float value)
{
	HydraTriggerChanged(controller, value);
}
void AHydraPlayerController::EventHydraBumperPressed(int32 controller)
{
	HydraBumperPressed(controller);
}
void AHydraPlayerController::EventHydraBumperReleased(int32 controller)
{
	HydraBumperReleased(controller);
}
void AHydraPlayerController::EventHydraJoystickPressed(int32 controller)
{
	HydraJoystickPressed(controller);
}
void AHydraPlayerController::EventHydraJoystickReleased(int32 controller)
{
	HydraJoystickReleased(controller);
}
void AHydraPlayerController::EventHydraStartPressed(int32 controller)
{
	HydraStartPressed(controller);
}
void AHydraPlayerController::EventHydraStartReleased(int32 controller)
{
	HydraStartReleased(controller);
}

void AHydraPlayerController::EventHydraJoystickMoved(int32 controller, FVector2D movement)
{
	HydraJoystickMoved(controller, movement);
}
void AHydraPlayerController::EventHydraControllerMoved(int32 controller,
	FVector position, FVector velocity, FVector acceleration,
	FRotator rotation)
{
	HydraControllerMoved(controller, position, velocity, acceleration, rotation);
}

bool AHydraPlayerController::HydraIsAvailable()
{
	return HydraDelegate::HydraIsAvailable();
}
int32 AHydraPlayerController::HydraWhichHand(int32 controller)
{
	return HydraDelegate::HydraWhichHand(controller);
}

/** Poll for latest data.*/
bool AHydraPlayerController::HydraGetLatestData(int32 controller, FVector& position, FVector& velocity, FVector& acceleration, FRotator& rotation,
	FVector2D& joystick, int32& buttons, float& trigger, bool& docked)
{
	if (controller > 4 || controller < 0){ return false; }

	return HydraGetHistoricalData(controller, 0, position, velocity, acceleration, rotation, joystick, buttons, trigger, docked);
}

/** Poll for historical data. Valid index is 0-9 */
bool AHydraPlayerController::HydraGetHistoricalData(int32 controller, int32 historyIndex, FVector& position, FVector& velocity, FVector& acceleration, FRotator& rotation,
	FVector2D& joystick, int32& buttons, float& trigger, bool& docked)
{
	if (historyIndex<0 || historyIndex>9){ return false; }
	if (controller > 4 || controller < 0){ return false; }

	sixenseControllerDataUE* data = &HydraHistoryData[historyIndex].controllers[controller];

	position = data->position;
	velocity = data->velocity;
	acceleration = data->acceleration;
	rotation = FRotator(data->rotation);
	joystick = data->joystick;
	buttons = data->buttons;
	trigger = data->trigger;
	docked = data->is_docked;
	return data->enabled;
}

//Required Overrides
void AHydraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IHydraPlugin::IsAvailable())
	{
		//Required to Work - Set self as a delegate
		IHydraPlugin::Get().SetDelegate((HydraDelegate*)this);
	}
}

void AHydraPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IHydraPlugin::IsAvailable())
	{
		//Required to Work - This is the plugin magic
		IHydraPlugin::Get().HydraTick(DeltaTime);
	}
}

