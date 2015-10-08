#pragma once

#include "HydraPluginPrivatePCH.h"
#include "HydraEnum.h"
#include "HydraComponent.generated.h"

//These macros cannot be multi-line or it will not compile
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHydraPluggedInSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHydraUnPluggedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHydraDockedSignature, class UHydraSingleController*, controller);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHydraUnDockedSignature, class UHydraSingleController*, controller);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHydraButtonPressedSignature, class UHydraSingleController*, controller, EHydraControllerButton, button);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHydraButtonReleasedSignature, class UHydraSingleController*, controller, EHydraControllerButton, button);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHydraJoystickMovedSignature, class UHydraSingleController*, controller, FVector2D, movement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FHydraControllerMovedSignature, class UHydraSingleController*, controller, FVector, position, FVector, velocity, FVector, acceleration, FRotator, orientation, FRotator, angularVelocity);

UCLASS(ClassGroup="Input Controller", meta=(BlueprintSpawnableComponent))
class UHydraPluginComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:

	//Assignable Events, used for fine-grained control
	//Buttons are relegated to input mapping, non-button features are available here

	UPROPERTY(BlueprintAssignable, Category = "Hydra Events")
	FHydraPluggedInSignature PluggedIn;

	UPROPERTY(BlueprintAssignable, Category = "Hydra Events")
	FHydraUnPluggedSignature Unplugged;

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

	// Requires SetMeshComponentLinks to be called prior (e.g. in BeginPlay), then if true these will auto-hide when docked
	UPROPERTY(EditAnywhere, Category = "Hydra Properties")
	bool HideMeshComponentsWhenDocked;

	// Once set, HideStaticMeshWhenDocked=true property will cause the bound components to hide.
	UFUNCTION(BlueprintCallable, Category = "Hydra Functions")
	void SetMeshComponentLinks(UMeshComponent* LeftMesh, UMeshComponent* RightMesh);

	//Due to custom logic, call this instead of multi-cast directly
	void Docked(UHydraSingleController* controller);
	void Undocked(UHydraSingleController* controller);

	//Callable Blueprint functions - Need to be defined for direct access
	/** Check if the hydra is available/plugged in.*/
	UFUNCTION(BlueprintCallable, Category = HydraFunctions)
	bool IsAvailable();

	//** Poll for historical data.  Valid Hand is Left or Right, Valid history index is 0-9.  */
	UFUNCTION(BlueprintCallable, Category = HydraFunctions)
	UHydraSingleController* GetHistoricalFrameForHand(EHydraControllerHand hand = HYDRA_HAND_LEFT, int32 historyIndex = 0);

	//** Get the latest available data given in a single frame. Valid Hand is Left or Right  */
	UFUNCTION(BlueprintCallable, Category = HydraFunctions)
	UHydraSingleController* GetLatestFrameForHand(EHydraControllerHand hand = HYDRA_HAND_LEFT);

	// Set a manual offset, use this for manual calibration
	UFUNCTION(BlueprintCallable, Category = HydraFunctions)
	void SetBaseOffset(FVector Offset);

	// Use in-built calibration. Expects either a T-Pose. If offset is provided it will add the given offset to the final calibration.
	// For T-pose the function defaults to 40cm height. At 0,0,0 this will simply calibrate the zero position
	UFUNCTION(BlueprintCallable, Category = HydraFunctions)
	void Calibrate(FVector OffsetFromShoulderMidPoint = FVector(0,0,40));


protected:
	UMeshComponent* LeftMeshComponent;
	UMeshComponent* RightMeshComponent;

	//Utility Functions
	int32 ControllerIdForHand(EHydraControllerHand hand);

	/** Poll for historical data. Valid ControllerId is 0 or 1, Valid history index is 0-9.*/
	UHydraSingleController* GetHistoricalFrameForControllerId(int32 controllerId, int32 historyIndex);

	//** Get the latest available data given in a single frame. Valid ControllerId is 0 or 1  */
	UHydraSingleController* GetLatestFrameForControllerId(int32 controllerId);

	class HydraDataDelegate* dataDelegate;

	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
};