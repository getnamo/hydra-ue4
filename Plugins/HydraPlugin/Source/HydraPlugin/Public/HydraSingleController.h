#pragma once

#include "HydraEnum.h"
#include "HydraSingleController.generated.h"

UCLASS(BlueprintType)
class UHydraSingleController : public UObject
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Frame")
	int32 controllerId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Frame")
	FVector position;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Frame")
	FVector rawPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Frame")
	FVector velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Frame")
	FVector acceleration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Frame")
	FRotator orientation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Frame")
	FRotator angularVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Frame")
	FVector2D joystick;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Frame")
	float trigger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Frame")
	bool b1Pressed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Frame")
	bool b2Pressed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Frame")
	bool b3Pressed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Frame")
	bool b4Pressed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Frame")
	bool startPressed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Frame")
	bool bumperPressed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Frame")
	bool triggerPressed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Frame")
	bool joystickClicked;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Frame")
	bool docked;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hydra Frame")
	TEnumAsByte<EHydraControllerHand> handPossession;

	//Convenience Call, optionally check hand possession property
	UFUNCTION(BlueprintCallable, Category = "Hydra Frame")
	bool isLeftHand();

	//Convenience Call, optionally check hand possession property
	UFUNCTION(BlueprintCallable, Category = "Hydra Frame")
	bool isRightHand();

	//Conversion
	void setFromSixenseDataUE(sixenseControllerDataUE* data);
};