#pragma once

#include "GameFramework/Actor.h"
#include "HydraBlueprintDelegate.h"
#include "HydraPluginActor.generated.h"



/**
 * Placeable Actor Example that receives Hydra input,
 * override any delegate functions to receive notifications.
 * Calling super is not necessary.
 *
 * Copy implementations to receive same functionality in a different class.
 */

UCLASS()
class AHydraPluginActor : public AActor, public HydraBlueprintDelegate, public IHydraInterface //Multiple Inheritance, add HydraDelegate inheritance to inherit all delegate functionality
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


