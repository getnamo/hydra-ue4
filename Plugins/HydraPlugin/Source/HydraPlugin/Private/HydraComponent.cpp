#include "HydraPluginPrivatePCH.h"
#include "HydraComponent.h"
#include "HydraSingleController.h"
#include "IHydraPlugin.h"
#include "HydraDataDelegate.h"
#include "Engine.h"
#include "CoreUObject.h"

UHydraPluginComponent::UHydraPluginComponent(const FObjectInitializer &init) : UActorComponent(init)
{
	bWantsInitializeComponent = true;
	bAutoActivate = true;
	//PrimaryComponentTick.bCanEverTick = true;	//the component automatically ticks so we don't have to
}

void UHydraPluginComponent::InitializeComponent()
{
	Super::InitializeComponent();

	//Attach delegate references
	dataDelegate = IHydraPlugin::Get().DataDelegate();
	dataDelegate->AddEventDelegate(this);
}

void UHydraPluginComponent::UninitializeComponent()
{
	//remove ourselves from the delegates
	dataDelegate->RemoveEventDelegate(this);
	dataDelegate = nullptr;

	Super::UninitializeComponent();
}

//Utility Functions
bool UHydraPluginComponent::IsAvailable()
{
	return IHydraPlugin::IsAvailable();
}

UHydraSingleController* UHydraPluginComponent::GetHistoricalFrameForControllerId(int32 controllerId, int32 historyIndex)
{
	//return HydraGetHistoricalFrames(controllerId, historyIndex);
	sixenseControllerDataUE* dataUE = dataDelegate->HydraGetHistoricalData(controllerId, historyIndex);

	UHydraSingleController* controller = NewObject<UHydraSingleController>(UHydraSingleController::StaticClass());
	controller->setFromSixenseDataUE(dataUE);
	return controller;
	return nullptr;
}

UHydraSingleController* UHydraPluginComponent::GetLatestFrameForControllerId(int32 controllerId)
{
	return GetHistoricalFrameForControllerId(controllerId, 0);
}

//Public Implementation

UHydraSingleController* UHydraPluginComponent::GetLatestFrameForHand(HydraControllerHand hand)
{
	return GetHistoricalFrameForControllerId(ControllerIdForHand(hand), 0);
}

//Calibration
void UHydraPluginComponent::SetBaseOffset(FVector Offset)
{
	dataDelegate->baseOffset = Offset;
}

void UHydraPluginComponent::Calibrate(FVector OffsetFromShouldMidPoint)
{
	//Get left and right hand midpoint position
	dataDelegate->baseOffset = -(dataDelegate->LeftController->rawPosition + dataDelegate->RightController->rawPosition) / 2 + OffsetFromShouldMidPoint;
}

//Frames
UHydraSingleController* UHydraPluginComponent::GetHistoricalFrameForHand(HydraControllerHand hand, int32 historyIndex)
{
	return GetHistoricalFrameForControllerId(ControllerIdForHand(hand), historyIndex);
}

int32 UHydraPluginComponent::ControllerIdForHand(HydraControllerHand hand)
{
	switch (hand)
	{
	case HYDRA_HAND_LEFT:
		return dataDelegate->LeftControllerId;
		break;
	case HYDRA_HAND_RIGHT:
		return dataDelegate->RightControllerId;
		break;
	default:
		return 0;	//default to left
		break;
	}	
}


