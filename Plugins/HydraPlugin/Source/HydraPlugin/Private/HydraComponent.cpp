#include "HydraPluginPrivatePCH.h"
#include "HydraComponent.h"
#include "HydraInterface.h"
#include "Engine.h"
#include "CoreUObject.h"

UHydraComponent::UHydraComponent(const FPostConstructInitializeProperties &init) : UActorComponent(init)
{
	bWantsInitializeComponent = true;
	bAutoActivate = true;
	PrimaryComponentTick.bCanEverTick = true;	//the component automatically ticks so we don't have to
}

void UHydraComponent::OnRegister()
{
	Super::OnRegister();

	//Attach the delegate pointer automatically to the owner of the component
	ValidSelfPointer = this;
	SetInterfaceDelegate(GetOwner());
	HydraStartup();
}

void UHydraComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Forward the component tick
	HydraTick(DeltaTime);
}

//Blueprint exposing the HydraDelegate Methods
//Override Callable Functions - Required to forward their implementations in order to compile, cannot skip implementation or bp definition
bool UHydraComponent::IsAvailable()
{
	return HydraBlueprintDelegate::HydraIsAvailable();
}

UHydraSingleController* UHydraComponent::GetHistoricalFrames(int32 controllerId, int32 historyIndex)
{
	return HydraBlueprintDelegate::HydraGetHistoricalFrames(controllerId, historyIndex);
}

UHydraSingleController* UHydraComponent::GetLatestFrame(int32 controllerId)
{
	return HydraBlueprintDelegate::HydraGetHistoricalFrames(controllerId, 0);
}


