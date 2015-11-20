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

void UHydraPluginComponent::SetMeshComponentLinks(UMeshComponent* PassedLeftMesh, UMeshComponent* PassedRightMesh)
{
	LeftMeshComponent = PassedLeftMesh;
	RightMeshComponent = PassedRightMesh;
}

void UHydraPluginComponent::Docked(UHydraSingleController* controller)
{
	//Check possession and auto-hide if enabled
	if (HideMeshComponentsWhenDocked)
	{
		switch (controller->handPossession)
		{
		case HYDRA_HAND_LEFT:
			if (LeftMeshComponent != nullptr)
				LeftMeshComponent->SetHiddenInGame(true);
			break;
		case HYDRA_HAND_RIGHT:
			if (RightMeshComponent != nullptr)
				RightMeshComponent->SetHiddenInGame(true);
			break;
		default:
			break;
		}
	}

	//Emit our multi-cast delegate
	ControllerDocked.Broadcast(controller);
}

void UHydraPluginComponent::Undocked(UHydraSingleController* controller)
{
	//Check possession and auto-hide if enabled
	if (HideMeshComponentsWhenDocked){

		switch (controller->handPossession)
		{
		case HYDRA_HAND_LEFT:
			if (LeftMeshComponent != nullptr)
				LeftMeshComponent->SetHiddenInGame(false);
			break;
		case HYDRA_HAND_RIGHT:
			if (RightMeshComponent != nullptr)
				RightMeshComponent->SetHiddenInGame(false);
			break;
		default:
			break;
		}
	}

	//Emit our multi-cast delegate
	ControllerUndocked.Broadcast(controller);
}

//Utility Functions
bool UHydraPluginComponent::IsAvailable()
{
	return IHydraPlugin::IsAvailable();
}

UHydraSingleController* UHydraPluginComponent::GetHistoricalFrameForControllerId(int32 controllerId, int32 historyIndex)
{
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
//Frames
UHydraSingleController* UHydraPluginComponent::GetHistoricalFrameForHand(EHydraControllerHand hand, int32 historyIndex)
{
	return GetHistoricalFrameForControllerId(ControllerIdForHand(hand), historyIndex);
}

UHydraSingleController* UHydraPluginComponent::GetLatestFrameForHand(EHydraControllerHand hand)
{
	return GetHistoricalFrameForHand(hand, 0);
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

//Determining Hand
int32 UHydraPluginComponent::ControllerIdForHand(EHydraControllerHand hand)
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


