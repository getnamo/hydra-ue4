#include "HydraPluginPrivatePCH.h"

#include "Engine.h"
#include "GameFramework/Actor.h"
#include "IHydraPlugin.h"
#include "HydraDelegate.h"
#include "HydraPluginActor.h"

//Constructor/Initializer
AHydraPluginActor::AHydraPluginActor(const FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
}

//Override implementation
void AHydraPluginActor::EventHydraControllerEnabled(int32 controller)
{
	HydraControllerEnabled(controller);
}
void AHydraPluginActor::EventHydraControllerDisabled(int32 controller)
{
	HydraControllerDisabled(controller);
}
void AHydraPluginActor::EventHydraPluggedIn()
{
	HydraPluggedIn();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Hydra Plugged in.")));
}
void AHydraPluginActor::EventHydraUnplugged()
{
	HydraUnplugged();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Hydra Unplugged.")));
}
void AHydraPluginActor::EventHydraDocked(int32 controller)
{
	HydraDocked(controller);
}
void AHydraPluginActor::EventHydraUndocked(int32 controller)
{
	HydraUndocked(controller);
}
void AHydraPluginActor::EventHydraAnyButtonPressed(int32 controller)
{
	HydraAnyButtonPressed(controller);
}
void AHydraPluginActor::EventHydraB1Pressed(int32 controller)
{
	HydraB1Pressed(controller);
}
void AHydraPluginActor::EventHydraB1Released(int32 controller)
{
	HydraB1Released(controller);
}
void AHydraPluginActor::EventHydraB2Pressed(int32 controller)
{
	HydraB2Pressed(controller);
}
void AHydraPluginActor::EventHydraB2Released(int32 controller)
{
	HydraB2Released(controller);
}
void AHydraPluginActor::EventHydraB3Pressed(int32 controller)
{
	HydraB3Pressed(controller);
}
void AHydraPluginActor::EventHydraB3Released(int32 controller)
{
	HydraB3Released(controller);
}
void AHydraPluginActor::EventHydraB4Pressed(int32 controller)
{
	HydraB4Pressed(controller);
}
void AHydraPluginActor::EventHydraB4Released(int32 controller)
{
	HydraB4Released(controller);
}
void AHydraPluginActor::EventHydraTriggerPressed(int32 controller)
{
	HydraTriggerPressed(controller);
}
void AHydraPluginActor::EventHydraTriggerReleased(int32 controller)
{
	HydraTriggerReleased(controller);
}
void AHydraPluginActor::EventHydraTriggerChanged(int32 controller, float value)
{
	HydraTriggerChanged(controller, value);
}
void AHydraPluginActor::EventHydraBumperPressed(int32 controller)
{
	HydraBumperPressed(controller);
}
void AHydraPluginActor::EventHydraBumperReleased(int32 controller)
{
	HydraBumperReleased(controller);
}
void AHydraPluginActor::EventHydraJoystickPressed(int32 controller)
{
	HydraJoystickPressed(controller);
}
void AHydraPluginActor::EventHydraJoystickReleased(int32 controller)
{
	HydraJoystickReleased(controller);
}
void AHydraPluginActor::EventHydraStartPressed(int32 controller)
{
	HydraStartPressed(controller);
}
void AHydraPluginActor::EventHydraStartReleased(int32 controller)
{
	HydraStartReleased(controller);
}

void AHydraPluginActor::EventHydraJoystickMoved(int32 controller, FVector2D movement)
{
	HydraJoystickMoved(controller, movement);
}
void AHydraPluginActor::EventHydraControllerMoved(int32 controller,
	FVector position, FVector velocity, FVector acceleration,
	FRotator rotation)
{
	HydraControllerMoved(controller, position, velocity, acceleration, rotation);
}

bool AHydraPluginActor::HydraIsAvailable()
{
	return HydraDelegate::HydraIsAvailable();
}
int32 AHydraPluginActor::HydraWhichHand(int32 controller)
{
	return HydraDelegate::HydraWhichHand(controller);
}

/** Poll for latest data.*/
bool AHydraPluginActor::HydraGetLatestData(int32 controller, FVector& position, FVector& velocity, FVector& acceleration, FRotator& rotation,
	FVector2D& joystick, int32& buttons, float& trigger, bool& docked)
{
	if (controller > 4 || controller < 0){ return false; }

	return HydraGetHistoricalData(controller, 0, position, velocity, acceleration, rotation, joystick, buttons, trigger, docked);
}

/** Poll for historical data. Valid index is 0-9 */
bool AHydraPluginActor::HydraGetHistoricalData(int32 controller, int32 historyIndex, FVector& position, FVector& velocity, FVector& acceleration, FRotator& rotation,
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
void AHydraPluginActor::BeginPlay()
{
	Super::BeginPlay();

	if (IHydraPlugin::IsAvailable())
	{
		//Debug Display - You can safely remove these two lines, retained for lazy copy-paste
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Plugin Module Loaded."));

		//Required to Work - Set self as a delegate
		IHydraPlugin::Get().SetDelegate((HydraDelegate*)this);
	}
}

void AHydraPluginActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IHydraPlugin::IsAvailable())
	{
		//Required to Work - This is the plugin magic
		IHydraPlugin::Get().HydraTick(DeltaTime);
	}
}

