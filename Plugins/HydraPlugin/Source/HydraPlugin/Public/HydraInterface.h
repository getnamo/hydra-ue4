#pragma once

#include "HydraSingleController.h"
#include "HydraInterface.generated.h"

UINTERFACE(MinimalAPI)
class UHydraInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IHydraInterface
{
	GENERATED_IINTERFACE_BODY()

public:

	//Define blueprint events
	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraPluggedIn();
	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraUnplugged();
	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraControllerEnabled(UHydraSingleController* controller);
	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraControllerDisabled(UHydraSingleController* controller);

	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraDocked(UHydraSingleController* controller);
	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraUndocked(UHydraSingleController* controller);

	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraAnyButtonPressed(UHydraSingleController* controller);

	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraB1Pressed(UHydraSingleController* controller);
	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraB1Released(UHydraSingleController* controller);
	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraB2Pressed(UHydraSingleController* controller);
	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraB2Released(UHydraSingleController* controller);
	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraB3Pressed(UHydraSingleController* controller);
	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraB3Released(UHydraSingleController* controller);
	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraB4Pressed(UHydraSingleController* controller);
	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraB4Released(UHydraSingleController* controller);
	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraTriggerPressed(UHydraSingleController* controller);
	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraTriggerReleased(UHydraSingleController* controller);
	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraTriggerChanged(UHydraSingleController* controller, float value);
	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraBumperPressed(UHydraSingleController* controller);
	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraBumperReleased(UHydraSingleController* controller);
	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraJoystickPressed(UHydraSingleController* controller);
	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraJoystickReleased(UHydraSingleController* controller);
	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraStartPressed(UHydraSingleController* controller);
	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraStartReleased(UHydraSingleController* controller);

	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraJoystickMoved(UHydraSingleController* controller, FVector2D movement);
	UFUNCTION(BlueprintImplementableEvent, Category = "Hydra Interface Events")
	void HydraControllerMoved(	UHydraSingleController* controller,
								FVector position, FVector velocity, FVector acceleration,
								FRotator orientation, FRotator angularVelocity);

	virtual FString ToString();
};