#pragma once

#include "HydraPluginPrivatePCH.h"
#include "HydraControllerData.h"
#include "HydraControllerComponent.generated.h"

//These macros cannot be multi-line or it will not compile
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHydraPluggedInSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHydraUnPluggedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHydraDockedSignature, const FHydraControllerData&, Controller);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHydraUnDockedSignature, const FHydraControllerData&, Controller);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHydraButtonPressedSignature, const FHydraControllerData&, Controller, EHydraControllerButton, button);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHydraButtonReleasedSignature, const FHydraControllerData&, Controller, EHydraControllerButton, button);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHydraJoystickMovedSignature, const FHydraControllerData&, Controller, FVector2D, movement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHydraControllerMovedSignature, const FHydraControllerData&, Controller, FVector, Position, FRotator, Orientation);


/**
	Optional Hydra specific component which receives hydra specific events and can query specific data.
*/
UCLASS(ClassGroup="Input Controller", meta=(BlueprintSpawnableComponent))
class HYDRAPLUGIN_API UHydraControllerComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:

	//Assignable Events, used for fine-grained control
	//Buttons are relegated to input mapping, non-button features are available here

	UPROPERTY(BlueprintAssignable, Category = "Hydra Events")
	FHydraPluggedInSignature OnPluggedIn;

	UPROPERTY(BlueprintAssignable, Category = "Hydra Events")
	FHydraUnPluggedSignature OnUnplugged;

	UPROPERTY(BlueprintAssignable, Category = "Hydra Events")
	FHydraDockedSignature ControllerDocked;

	UPROPERTY(BlueprintAssignable, Category = "Hydra Events")
	FHydraUnDockedSignature ControllerUndocked;

	UPROPERTY(BlueprintAssignable, Category = "Hydra Events")
	FHydraButtonPressedSignature ButtonPressed;

	UPROPERTY(BlueprintAssignable, Category = "Hydra Events")
	FHydraButtonReleasedSignature ButtonReleased;

	UPROPERTY(BlueprintAssignable, Category = "Hydra Events")
	FHydraJoystickMovedSignature JoystickMoved;

	UPROPERTY(BlueprintAssignable, Category = "Hydra Events")
	FHydraControllerMovedSignature ControllerMoved;

	//Callable Blueprint functions - Need to be defined for direct access
	/** Check if the hydra is available/plugged in.*/
	UFUNCTION(BlueprintCallable, Category = HydraFunctions)
	bool IsAvailable();

	//** Get the latest available data given in a single frame. Valid Hand is Left or Right  */
	UFUNCTION(BlueprintCallable, Category = HydraFunctions)
	bool GetLatestFrameForHand(FHydraControllerData& OutControllerData, EHydraControllerHand hand = HYDRA_HAND_LEFT);

	// Set a manual offset, use this for manual calibration
	UFUNCTION(BlueprintCallable, Category = HydraFunctions)
	void SetBaseOffset(FVector Offset);

	// Use in-built calibration. Expects either a T-Pose. If offset is provided it will add the given offset to the final calibration.
	// For T-pose the function defaults to 40cm height. At 0,0,0 this will simply calibrate the zero position
	UFUNCTION(BlueprintCallable, Category = HydraFunctions)
	void Calibrate(FVector OffsetFromShoulderMidPoint = FVector(0,0,40));


protected:
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;

};