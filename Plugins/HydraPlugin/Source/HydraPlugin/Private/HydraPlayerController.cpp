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

//Override Callable Functions - Required to forward their implementations in order to compile, cannot skip implementation as we can for events.
bool AHydraPlayerController::HydraIsAvailable()
{
	return HydraDelegate::HydraIsAvailable();
}
int32 AHydraPlayerController::HydraWhichHand(int32 controller)
{
	return HydraDelegate::HydraWhichHand(controller);
}
bool AHydraPlayerController::HydraGetLatestData(int32 controller, FVector& position, FVector& velocity, FVector& acceleration, FRotator& rotation, FRotator& angularVelocity,
	FVector2D& joystick, int32& buttons, float& trigger, bool& docked)
{
	return HydraDelegate::HydraGetLatestData(controller, position, velocity, acceleration, rotation, angularVelocity, joystick, buttons, trigger, docked);
}
bool AHydraPlayerController::HydraGetHistoricalData(int32 controller, int32 historyIndex, FVector& position, FVector& velocity, FVector& acceleration, FRotator& rotation, FRotator& angularVelocity,
	FVector2D& joystick, int32& buttons, float& trigger, bool& docked)
{
	return HydraDelegate::HydraGetHistoricalData(controller, historyIndex, position, velocity, acceleration, rotation, angularVelocity, joystick, buttons, trigger, docked);
}

//Required Overrides, forward startup and tick.
void AHydraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	HydraStartup();
}

void AHydraPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HydraTick(DeltaTime);
}

