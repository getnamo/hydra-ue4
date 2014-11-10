#pragma once

#include "GameFramework/PlayerController.h"
#include "HydraBlueprintDelegate.h"
#include "HydraPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AHydraPlayerController : public APlayerController, public HydraBlueprintDelegate, public IHydraInterface
{
	GENERATED_UCLASS_BODY()

	//Callable Blueprint functions - Need to be defined for direct access
	/** Check if the hydra is available/plugged in.*/
	UFUNCTION(BlueprintCallable, Category = HydraFunctions)
	bool IsAvailable();

	/** Poll for historical data. Valid ControllerId is 0 or 1, Valid history index is 0-9.*/
	UFUNCTION(BlueprintCallable, Category = HydraFunctions)
	UHydraSingleController* GetHistoricalFrames(int32 controllerId, int32 historyIndex);

	//** Get the latest available data given in a single frame. Valid ControllerId is 0 or 1  */
	UFUNCTION(BlueprintCallable, Category = HydraFunctions)
	UHydraSingleController* GetLatestFrame(int32 controllerId);

	//Override Initialization and Tick to forward *required* hydra functions.
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
