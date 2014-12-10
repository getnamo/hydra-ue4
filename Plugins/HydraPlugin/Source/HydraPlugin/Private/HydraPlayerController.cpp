#include "HydraPluginPrivatePCH.h"

#include "IHydraPlugin.h"
#include "HydraDelegate.h"
#include "HydraPlayerController.h"

//Constructor/Initializer
AHydraPlayerController::AHydraPlayerController(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
}

//Blueprint exposing the HydraDelegate Methods
//Override Callable Functions - Required to forward their implementations in order to compile, cannot skip implementation or bp definition
bool AHydraPlayerController::IsAvailable()
{
	return HydraBlueprintDelegate::HydraIsAvailable();
}

UHydraSingleController* AHydraPlayerController::GetHistoricalFrames(int32 controllerId, int32 historyIndex)
{
	return HydraBlueprintDelegate::HydraGetHistoricalFrames(controllerId, historyIndex);
}

UHydraSingleController* AHydraPlayerController::GetLatestFrame(int32 controllerId)
{
	return HydraBlueprintDelegate::HydraGetHistoricalFrames(controllerId, 0);
}

//Required Overrides, forward startup and tick.
void AHydraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ValidSelfPointer = this;	//required from v0.7
	HydraStartup();
}

void AHydraPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HydraTick(DeltaTime);
}

