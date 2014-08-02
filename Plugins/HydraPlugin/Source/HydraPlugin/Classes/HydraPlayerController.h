#pragma once

#include "GameFramework/PlayerController.h"
#include "HydraDelegate.h"
#include "HydraPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AHydraPlayerController : public APlayerController, public HydraDelegate
{
	GENERATED_UCLASS_BODY()
	
	/**
	* We Override every delegate function to forward them to blueprint events,
	* but if you're extending C++ class yourself you can override just the ones
	* you are interested in.
	*/

	//Define blueprint events
	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraPluggedIn();
	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraUnplugged();
	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraControllerEnabled(int32 controller);
	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraControllerDisabled(int32 controller);

	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraDocked(int32 controller);
	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraUndocked(int32 controller);

	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraAnyButtonPressed(int32 controller);

	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraB1Pressed(int32 controller);
	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraB1Released(int32 controller);
	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraB2Pressed(int32 controller);
	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraB2Released(int32 controller);
	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraB3Pressed(int32 controller);
	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraB3Released(int32 controller);
	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraB4Pressed(int32 controller);
	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraB4Released(int32 controller);
	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraTriggerPressed(int32 controller);
	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraTriggerReleased(int32 controller);
	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraTriggerChanged(int32 controller, float value);
	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraBumperPressed(int32 controller);
	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraBumperReleased(int32 controller);
	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraJoystickPressed(int32 controller);
	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraJoystickReleased(int32 controller);
	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraStartPressed(int32 controller);
	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraStartReleased(int32 controller);

	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraJoystickMoved(int32 controller, FVector2D movement);
	UFUNCTION(BlueprintImplementableEvent, Category = HydraEvents)
		void HydraControllerMoved(int32 controller,
		FVector position, FVector velocity, FVector acceleration,
		FRotator rotation, FRotator angularVelocity);

	//Callable Blueprint functions

	/** Check if the hydra is available/plugged in.*/
	UFUNCTION(BlueprintCallable, Category = HydraFunctions)
		bool HydraIsAvailable();
	/** Detemine which hand you're holding the hydra in. Determined and reset on docking the controller.*/
	UFUNCTION(BlueprintCallable, Category = HydraFunctions)
		int32 HydraWhichHand(int32 controller);
	/** Poll for latest data. Returns false if data is unavailable.*/
	UFUNCTION(BlueprintCallable, Category = HydraFunctions)
		bool HydraGetLatestData(int32 controller, FVector& position, FVector& velocity, FVector& acceleration, FRotator& rotation, FRotator& angularVelocity,
		FVector2D& joystick, int32& buttons, float& trigger, bool& docked);
	/** Poll for historical data. Valid index is 0-9. Returns false if data is unavailable.*/
	UFUNCTION(BlueprintCallable, Category = HydraFunctions)
		bool HydraGetHistoricalData(int32 controller, int32 historyIndex, FVector& position, FVector& velocity, FVector& acceleration, FRotator& rotation, FRotator& angularVelocity,
		FVector2D& joystick, int32& buttons, float& trigger, bool& docked);

	//Override Initialization and Tick to forward *required* hydra functions.
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
