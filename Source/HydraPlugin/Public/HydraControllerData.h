#pragma once

#include "HydraEnum.h"
#include "HydraControllerData.generated.h"

USTRUCT(BlueprintType)
struct HYDRAPLUGIN_API FHydraControllerData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Controller Data")
	int32 ControllerId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Controller Data")
	FVector RawPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Controller Data")
	FVector Position;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Controller Data")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Controller Data")
	FVector Acceleration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Controller Data")
	FRotator Orientation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Controller Data")
	FRotator AngularVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Controller Data")
	FVector2D Joystick;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Controller Data")
	float Trigger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Controller Data")
	bool B1Pressed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Controller Data")
	bool B2Pressed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Controller Data")
	bool B3Pressed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Controller Data")
	bool B4Pressed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Controller Data")
	bool StartPressed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Controller Data")
	bool BumperPressed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Controller Data")
	bool TriggerPressed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Controller Data")
	bool JoystickClicked;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Controller Data")
	bool Docked;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Controller Data")
	bool Enabled;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Controller Data")
	TEnumAsByte<EHydraControllerHand> HandPossession;

	void SetFromSixenseDataUE(const SixenseControllerDataUE& data);
};