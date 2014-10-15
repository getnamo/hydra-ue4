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

//Blueprint exposing the HydraDelegate Methods
//Override Callable Functions - Required to forward their implementations in order to compile, cannot skip implementation or bp definition
bool AHydraPluginActor::IsAvailable()
{
	return HydraBlueprintDelegate::HydraIsAvailable();
}

UHydraSingleController* AHydraPluginActor::GetHistoricalFrames(int32 controllerId, int32 historyIndex)
{
	return HydraBlueprintDelegate::HydraGetHistoricalFrames(controllerId, historyIndex);
}

UHydraSingleController* AHydraPluginActor::GetLatestFrame(int32 controllerId)
{
	return HydraBlueprintDelegate::HydraGetHistoricalFrames(controllerId, 0);
}

//Required Overrides
void AHydraPluginActor::BeginPlay()
{
	Super::BeginPlay();

	//Actors by default aren't attached to the input chain, so we enable input for this actor to forward Key and Gamepad Events
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	EnableInput(PC);

	//Required Hydra Initialization
	ValidSelfPointer = this;	//required from v0.7
	HydraStartup();
}

void AHydraPluginActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Required Hydra Tick
	HydraTick(DeltaTime);
}

