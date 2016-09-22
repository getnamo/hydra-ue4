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
	return ( IHydraPlugin::IsAvailable() && 
			 IHydraPlugin::Get().IsPluggedInAndEnabled() );
}

bool UHydraPluginComponent::GetLatestFrameForHand(FHydraControllerData& OutControllerData, EHydraControllerHand hand /*= HYDRA_HAND_LEFT*/)
{
	switch (hand)
	{
	case HYDRA_HAND_LEFT:
		return IHydraPlugin::Get().LeftHandData(OutControllerData);
		break;
	case HYDRA_HAND_RIGHT:
		return IHydraPlugin::Get().RightHandData(OutControllerData);
		break;
	default:
		break;
	}
	return false;
}

//Calibration
void UHydraPluginComponent::SetBaseOffset(FVector Offset)
{
	FTransform CalibrationTransform;
	CalibrationTransform.SetLocation(Offset);
	IHydraPlugin::Get().SetCalibrationTransform(CalibrationTransform);
}

void UHydraPluginComponent::Calibrate(FVector OffsetFromShouldMidPoint)
{
	FTransform CalibrationTransform;

	FHydraControllerData Left, Right;
	bool ValidData = false;
	ValidData = IHydraPlugin::Get().LeftHandData(Left);
	ValidData = ValidData && IHydraPlugin::Get().LeftHandData(Right);
	
	//Get left and right hand midpoint position
	FVector Offset = -(Left.RawPosition + Right.RawPosition) / 2 + OffsetFromShouldMidPoint;
	
	CalibrationTransform.SetLocation(Offset);
	IHydraPlugin::Get().SetCalibrationTransform(CalibrationTransform);
}


