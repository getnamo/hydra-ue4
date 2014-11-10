#pragma once

#include "HydraPluginPrivatePCH.h"
#include "HydraBlueprintDelegate.h"
#include "HydraComponent.generated.h"

UCLASS(ClassGroup="Input Controller", meta=(BlueprintSpawnableComponent))
class UHydraComponent : public UActorComponent, public HydraBlueprintDelegate
{
	GENERATED_UCLASS_BODY()

public:

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

	virtual void OnRegister() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
};