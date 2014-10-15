#include "HydraPluginPrivatePCH.h"
#include "HydraBlueprintDelegate.h"

//DefaultPCIP
/*HydraBlueprintDelegate::HydraBlueprintDelegate(const FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
}*/

//Event forwarding to interface
void HydraBlueprintDelegate::HydraPluggedIn()
{
	IHydraInterface::Execute_HydraPluggedIn(_interfaceDelegate);
}
void HydraBlueprintDelegate::HydraUnplugged()
{
	IHydraInterface::Execute_HydraUnplugged(_interfaceDelegate);
}
void HydraBlueprintDelegate::HydraControllerEnabled(int32 controller)
{
	IHydraInterface::Execute_HydraControllerEnabled(_interfaceDelegate, _latestFrame[controller]);
}
void HydraBlueprintDelegate::HydraControllerDisabled(int32 controller)
{
	IHydraInterface::Execute_HydraControllerDisabled(_interfaceDelegate, _latestFrame[controller]);
}
void HydraBlueprintDelegate::HydraDocked(int32 controller)
{
	IHydraInterface::Execute_HydraDocked(_interfaceDelegate, _latestFrame[controller]);
}
void HydraBlueprintDelegate::HydraUndocked(int32 controller)
{
	IHydraInterface::Execute_HydraUndocked(_interfaceDelegate, _latestFrame[controller]);
}
void HydraBlueprintDelegate::HydraAnyButtonPressed(int32 controller)
{
	IHydraInterface::Execute_HydraAnyButtonPressed(_interfaceDelegate, _latestFrame[controller]);
}
void HydraBlueprintDelegate::HydraB1Pressed(int32 controller)
{
	IHydraInterface::Execute_HydraB1Pressed(_interfaceDelegate, _latestFrame[controller]);
}
void HydraBlueprintDelegate::HydraB1Released(int32 controller)
{
	IHydraInterface::Execute_HydraB1Released(_interfaceDelegate, _latestFrame[controller]);
}
void HydraBlueprintDelegate::HydraB2Pressed(int32 controller)
{
	IHydraInterface::Execute_HydraB2Pressed(_interfaceDelegate, _latestFrame[controller]);
}
void HydraBlueprintDelegate::HydraB2Released(int32 controller)
{
	IHydraInterface::Execute_HydraB2Released(_interfaceDelegate, _latestFrame[controller]);
}
void HydraBlueprintDelegate::HydraB3Pressed(int32 controller)
{
	IHydraInterface::Execute_HydraB3Pressed(_interfaceDelegate, _latestFrame[controller]);
}
void HydraBlueprintDelegate::HydraB3Released(int32 controller)
{
	IHydraInterface::Execute_HydraB3Released(_interfaceDelegate, _latestFrame[controller]);
}
void HydraBlueprintDelegate::HydraB4Pressed(int32 controller)
{
	IHydraInterface::Execute_HydraB4Pressed(_interfaceDelegate, _latestFrame[controller]);
}
void HydraBlueprintDelegate::HydraB4Released(int32 controller)
{
	IHydraInterface::Execute_HydraB4Released(_interfaceDelegate, _latestFrame[controller]);
}
void HydraBlueprintDelegate::HydraTriggerPressed(int32 controller)
{
	IHydraInterface::Execute_HydraTriggerPressed(_interfaceDelegate, _latestFrame[controller]);
}
void HydraBlueprintDelegate::HydraTriggerReleased(int32 controller)
{
	IHydraInterface::Execute_HydraTriggerReleased(_interfaceDelegate, _latestFrame[controller]);
}
void HydraBlueprintDelegate::HydraTriggerChanged(int32 controller, float value)
{
	IHydraInterface::Execute_HydraTriggerChanged(_interfaceDelegate, _latestFrame[controller], value);
}
void HydraBlueprintDelegate::HydraBumperPressed(int32 controller)
{
	IHydraInterface::Execute_HydraBumperPressed(_interfaceDelegate, _latestFrame[controller]);
}
void HydraBlueprintDelegate::HydraBumperReleased(int32 controller)
{
	IHydraInterface::Execute_HydraBumperReleased(_interfaceDelegate, _latestFrame[controller]);
}
void HydraBlueprintDelegate::HydraJoystickPressed(int32 controller)
{
	IHydraInterface::Execute_HydraJoystickPressed(_interfaceDelegate, _latestFrame[controller]);
}
void HydraBlueprintDelegate::HydraJoystickReleased(int32 controller)
{
	IHydraInterface::Execute_HydraJoystickReleased(_interfaceDelegate, _latestFrame[controller]);
}
void HydraBlueprintDelegate::HydraStartPressed(int32 controller)
{
	IHydraInterface::Execute_HydraStartPressed(_interfaceDelegate, _latestFrame[controller]);
}
void HydraBlueprintDelegate::HydraStartReleased(int32 controller)
{
	IHydraInterface::Execute_HydraStartReleased(_interfaceDelegate, _latestFrame[controller]);
}
void HydraBlueprintDelegate::HydraJoystickMoved(int32 controller, FVector2D movement)
{
	IHydraInterface::Execute_HydraJoystickMoved(_interfaceDelegate, _latestFrame[controller], movement);
}
void HydraBlueprintDelegate::HydraControllerMoved(int32 controller,
	FVector position, FVector velocity, FVector acceleration,
	FRotator rotation, FRotator angularVelocity)
{
	IHydraInterface::Execute_HydraControllerMoved(_interfaceDelegate, _latestFrame[controller], position, velocity, acceleration, rotation, angularVelocity);
}

//Blueprint exposing the HydraDelegate Methods
//Override Callable Functions - Required to forward their implementations in order to compile, cannot skip implementation as we can for events.
bool HydraBlueprintDelegate::HydraIsAvailable()
{
	return HydraDelegate::HydraIsAvailable();
}

UHydraSingleController* HydraBlueprintDelegate::HydraGetHistoricalFrames(int32 controllerId, int32 historyIndex)
{
	sixenseControllerDataUE* dataUE = HydraDelegate::HydraGetHistoricalData(controllerId, historyIndex);

	UHydraSingleController* controller;
	controller = ConstructObject<UHydraSingleController>(UHydraSingleController::StaticClass());
	controller->setFromSixenseDataUE(dataUE);
	return controller;
}

UHydraSingleController* HydraBlueprintDelegate::HydraGetLatestFrame(int32 controllerId)
{
	return HydraGetHistoricalFrames(controllerId, 0);
}


//Setting
void HydraBlueprintDelegate::SetInterfaceDelegate(UObject* newDelegate)
{
	UE_LOG(LogClass, Log, TEXT("InterfaceDelegate passed: %s"), *newDelegate->GetName());

	//Use this format to support both blueprint and C++ form
	if (newDelegate->GetClass()->ImplementsInterface(UHydraInterface::StaticClass()))
	{
		_interfaceDelegate = newDelegate;
	}
	else
	{
		//Try casting as self
		if (ValidSelfPointer->GetClass()->ImplementsInterface(UHydraInterface::StaticClass()))
		{
			_interfaceDelegate = (UObject*)this;
		}
		else
		{
			//If you're crashing its probably because of this setting causing an assert failure
			_interfaceDelegate = NULL;
		}

		//Either way post a warning, this will be a common error
		UE_LOG(LogClass, Log, TEXT("HydraBlueprintDelegate Warning: Delegate is NOT set, did your class implement HydraInterface?"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("HydraBlueprintDelegate Warning: Delegate is NOT set, did your class implement HydraInterface?"));
	}
}

//Startup
void HydraBlueprintDelegate::HydraStartup()
{
	HydraDelegate::HydraStartup();

	UObject* validUObject = NULL;
	validUObject = Cast<UObject>(ValidSelfPointer);

	//Setup our Controller BP array
	for (int i = 0; i < MAX_CONTROLLERS_SUPPORTED; i++)
	{
		UHydraSingleController* controller;
		if (validUObject)
			controller = NewObject<UHydraSingleController>(validUObject, UHydraSingleController::StaticClass());
		else
			controller = NewObject<UHydraSingleController>(UHydraSingleController::StaticClass());	//no ownership
		_latestFrame.Add(controller);
	}

	//Set self as interface delegate by default
	if (!_interfaceDelegate && validUObject)
		SetInterfaceDelegate(validUObject);
}

void HydraBlueprintDelegate::HydraTick(float DeltaTime)
{
	HydraDelegate::HydraTick(DeltaTime);

	//Sync our array
	for (int i = 0; i < MAX_CONTROLLERS_SUPPORTED; i++)
	{
		UHydraSingleController* controller = _latestFrame[i];
		controller->setFromSixenseDataUE(HydraDelegate::HydraGetLatestData(i));
		controller->controllerId = i;
	}
}