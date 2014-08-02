#include "HydraPluginPrivatePCH.h"

#include "Engine.h"
#include "GameFramework/Actor.h"
#include "HydraDelegate.h"
#include "HydraPluginActor.h"

//Constructor/Initializer
AHydraPluginActor::AHydraPluginActor(const FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
}

//Override Callable Functions - Required to forward their implementations in order to compile, cannot skip implementation as we can for events.
bool AHydraPluginActor::HydraIsAvailable()
{
	return HydraDelegate::HydraIsAvailable();
}
int32 AHydraPluginActor::HydraWhichHand(int32 controller)
{
	return HydraDelegate::HydraWhichHand(controller);
}
bool AHydraPluginActor::HydraGetLatestData(int32 controller, FVector& position, FVector& velocity, FVector& acceleration, FRotator& rotation, FRotator& angularVelocity,
	FVector2D& joystick, int32& buttons, float& trigger, bool& docked)
{
	return HydraDelegate::HydraGetLatestData(controller, position, velocity, acceleration, rotation, angularVelocity, joystick, buttons, trigger, docked);
}
bool AHydraPluginActor::HydraGetHistoricalData(int32 controller, int32 historyIndex, FVector& position, FVector& velocity, FVector& acceleration, FRotator& rotation, FRotator& angularVelocity,
	FVector2D& joystick, int32& buttons, float& trigger, bool& docked)
{
	return HydraDelegate::HydraGetHistoricalData(controller, historyIndex, position, velocity, acceleration, rotation, angularVelocity, joystick, buttons, trigger, docked);
}

//Required Overrides
void AHydraPluginActor::BeginPlay()
{
	Super::BeginPlay();

	//Actors by default aren't attached to the input chain, so we enable input for this actor to forward Key and Gamepad Events
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	EnableInput(PC);

	//Required Hydra Initialization
	HydraStartup();
}

void AHydraPluginActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Required Hydra Tick
	HydraTick(DeltaTime);
}

