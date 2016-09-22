#include "HydraPluginPrivatePCH.h"
#include "HydraComponent.h"
#include "IHydraPlugin.h"
#include "Engine.h"


UHydraPluginComponent::UHydraPluginComponent(const FObjectInitializer &init) : UActorComponent(init)
{
	bWantsInitializeComponent = true;
	bAutoActivate = true;
}

void UHydraPluginComponent::InitializeComponent()
{
	Super::InitializeComponent();

	//Attach delegate references
	IHydraPlugin::Get().AddComponentDelegate(this);
}

void UHydraPluginComponent::UninitializeComponent()
{
	//remove ourselves from the delegates
	IHydraPlugin::Get().RemoveComponentDelegate(this);

	Super::UninitializeComponent();
}

//Utility Functions
bool UHydraPluginComponent::IsAvailable()
{
	return IHydraPlugin::IsAvailable();
}

void UHydraPluginComponent::GetHistoricalFrameForHand(FHydraControllerData& OutControllerData, EHydraControllerHand hand /*= HYDRA_HAND_LEFT*/, int32 historyIndex /*= 0*/)
{

}

void UHydraPluginComponent::GetLatestFrameForHand(FHydraControllerData& OutControllerData, EHydraControllerHand hand /*= HYDRA_HAND_LEFT*/)
{
	GetHistoricalFrameForHand(OutControllerData, hand, 0);
}

//Calibration
void UHydraPluginComponent::SetBaseOffset(FVector Offset)
{
	//dataDelegate->baseOffset = Offset;
}

void UHydraPluginComponent::Calibrate(FVector OffsetFromShouldMidPoint)
{
	//Get left and right hand midpoint position
	//dataDelegate->baseOffset = -(dataDelegate->LeftController->rawPosition + dataDelegate->RightController->rawPosition) / 2 + OffsetFromShouldMidPoint;
}

//Determining Hand
int32 UHydraPluginComponent::ControllerIdForHand(EHydraControllerHand hand)
{
	return 0;
	/*switch (hand)
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
	}*/	
}


