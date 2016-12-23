#include "HydraPluginPrivatePCH.h"

#include "IHydraPlugin.h"
#include "IMotionController.h"

#include "SlateBasics.h"
#include "IPluginManager.h"
#include "HydraControllerComponent.h"
#include "HydraUtility.h"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <sixense.h>
#include <windows.h>

#define LOCTEXT_NAMESPACE "HydraPlugin"
#define HYDRA_HISTORY_MAX 3				//frame history for data, shrunken to minimum 3 frame history for acceleration
DEFINE_LOG_CATEGORY_STATIC(HydraPluginLog, Log, All);

//Private API - This is where the magic happens

#pragma region DLLImport

//DLL import definition
typedef int (*dll_sixenseInit)(void);
typedef int (*dll_sixenseExit)(void);
typedef int (*dll_sixenseGetAllNewestData)(sixenseAllControllerData *);

dll_sixenseInit HydraInit;
dll_sixenseExit HydraExit;
dll_sixenseGetAllNewestData HydraGetAllNewestData;

#pragma endregion DLLImport

#pragma region FHydraPlugin - Declaration

//Forward declaration
class FHydraPlugin : public IHydraPlugin
{
	FTransform CalibrationTransform;
	class FHydraController* controllerReference = nullptr;
	TArray<UHydraControllerComponent*> ComponentDelegates;
	bool inputDeviceCreated = false;

public:

	virtual TSharedPtr< class IInputDevice > CreateInputDevice(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) override;

	virtual void AddComponentDelegate(UHydraControllerComponent* delegateComponent) override;

	virtual void RemoveComponentDelegate(UHydraControllerComponent* delegateComponent) override;

	virtual void SetCalibrationTransform(const FTransform& InCalibrationTransform) override;

	virtual FTransform GetCalibrationTransform() override;

	virtual bool IsPluggedInAndEnabled() override;
	virtual bool LeftHandData(FHydraControllerData& OutData) override;
	virtual bool RightHandData(FHydraControllerData& OutData) override;

	//Call this function on all the attached plugin component delegates
	void CallFunctionOnDelegates(TFunction< void(UHydraControllerComponent*)> InFunction);
};

#pragma endregion FHydraPlugin - Declaration

#pragma region DataCollector

//Collector class contains all the data captured from .dll and delegate data will point to this structure (allDataUE and historicalDataUE).
class DataCollector
{
public:
	HydraUtilityTimer DeltaTimer;

	DataCollector() :
		LatestLeft(nullptr),
		LatestRight(nullptr)
	{
		DeltaTimer.tick();
	}

	

	SixenseControllerDataUE ConvertData(sixenseControllerData* data, FVector offset = FVector(0,0,0))
	{
		SixenseControllerDataUE converted;

		//Convert Sixense Axis to Unreal: UnrealX = - SixenseZ   UnrealY = SixenseX   UnrealZ = SixenseY
		converted.rawPosition = FVector(-data->pos[2] / 10, data->pos[0] / 10, data->pos[1] / 10);					//converted to cm from mm
		converted.position = converted.rawPosition + offset;							
		converted.quat = FQuat(data->rot_quat[2], -data->rot_quat[0], -data->rot_quat[1], data->rot_quat[3]);		//converted & rotation values inverted
		converted.rotation = FRotator(converted.quat);																//convert once and re-use in blueprints
		converted.joystick = FVector2D(data->joystick_x, data->joystick_y);
		converted.trigger = data->trigger;
		converted.trigger_pressed = (converted.trigger > 0.5f);
		converted.buttons = data->buttons;
		converted.sequence_number = data->sequence_number;
		converted.firmware_revision = data->firmware_revision;
		converted.hardware_revision = data->hardware_revision;
		converted.packet_type = data->packet_type;
		converted.magnetic_frequency = data->magnetic_frequency;
		converted.enabled = (data->enabled == 1);
		converted.controller_index = data->controller_index;
		converted.is_docked = (data->is_docked != 0);
		converted.which_hand = data->which_hand;
		converted.hemi_tracking_enabled = (data->hemi_tracking_enabled != 0);

		return converted;
	}

	//todo: add yaw rotational offset support?
	void ConvertAllData(FVector offset = FVector(0, 0, 0))
	{
		AllDataUE.enabledCount = 0;

		for (int i = 0; i < MAX_CONTROLLERS_SUPPORTED; i++)
		{
			AllDataUE.controllers[i] = ConvertData(&AllSixenseData.controllers[i], offset);
			if (AllDataUE.controllers[i].enabled){
				AllDataUE.enabledCount++;
			}

			//update latest pointers
			if (AllDataUE.controllers[i].which_hand == (uint8)EHydraControllerHand::HYDRA_HAND_LEFT)
			{
				LatestLeft = &AllDataUE.controllers[i];
			}
			else if (AllDataUE.controllers[i].which_hand == (uint8)EHydraControllerHand::HYDRA_HAND_RIGHT)
			{
				LatestRight = &AllDataUE.controllers[i];
			}
		}
	}

	void UpdateDerivedValues(float DeltaTime)
	{
		for (int i = 0; i < MAX_CONTROLLERS_SUPPORTED; i++)
		{
			SixenseControllerDataUE& Current = AllDataUE.controllers[i];
			SixenseControllerDataUE& Last = HistoricalDataUE[1].controllers[i];

			Current.velocity = (Current.position - Last.position) / DeltaTime;
			Current.acceleration = (Current.velocity - Last.velocity) / DeltaTime;
			Current.angular_velocity = (Current.rotation - Last.rotation) * (1/DeltaTime);
		}
	}

	void GetLatestData(const FVector& Offset)
	{
		int success = HydraGetAllNewestData(&AllSixenseData);
		if (success == SIXENSE_FAILURE) {
			UE_LOG(HydraPluginLog, Error, TEXT("Hydra Error! Failed to get freshest data."));
			return;
		}
		//if the hydras are unavailable don't try to get more information
		if (!AllDataUE.available) {
			UE_LOG(HydraPluginLog, Log, TEXT("Collector data not available."));
			return;
		}

		float DeltaTime = DeltaTimer.tock();
		DeltaTimer.tick();

		ConvertAllData(Offset);

		//Set all historical data
		for (int i = 0; i < HYDRA_HISTORY_MAX-1; i++)
		{
			HistoricalDataUE[i+1] = HistoricalDataUE[i];
		}

		//latest
		HistoricalDataUE[0] = AllDataUE;

		//update velocities/acceleration
		UpdateDerivedValues(DeltaTime);
		
	}

	sixenseAllControllerData AllSixenseData;
	SixenseControllerDataUE* LatestLeft;
	SixenseControllerDataUE* LatestRight;

	SixenseAllControllerDataUE AllDataUE;
	SixenseAllControllerDataUE HistoricalDataUE[HYDRA_HISTORY_MAX];
};

#pragma endregion DataCollector

#pragma region FHydraController

class FHydraPlugin;

struct FHydraKeyMap 
{
	FKey HydraKey;
	FKey MotionControllerKey;

	FHydraKeyMap(const FKey& InHydra, const FKey& InMCKey)
	{
		HydraKey = InHydra;
		MotionControllerKey = InMCKey;
	}
};

class FHydraController : public IInputDevice, public IMotionController
{	

public:
	DataCollector* collector;
	FHydraPlugin* pluginPointer;

	void* DLLHandle;
	HydraUtilityTimer UtilityTimer;

	/** handler to send all messages to */
	TSharedRef<FGenericApplicationMessageHandler> MessageHandler;

	TArray<FHydraKeyMap> LeftKeyMap;
	TArray<FHydraKeyMap> RightKeyMap;
	
	void AddInputMappingKeys()
	{
		//Docking
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftDocked, LOCTEXT("HydraLeftDocked", "Hydra (L) Docked"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightDocked, LOCTEXT("HydraRightDocked", "Hydra (R) Docked"), FKeyDetails::GamepadKey));

		//Left
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftJoystickX, LOCTEXT("HydraLeftJoystickX", "Hydra (L) Joystick X"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftJoystickY, LOCTEXT("HydraLeftJoystickY", "Hydra (L) Joystick Y"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftJoystickClick, LOCTEXT("HydraLeftJoystickClick", "Hydra (L) Joystick Click"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftB1, LOCTEXT("HydraLeftB1", "Hydra (L) B1"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftB2, LOCTEXT("HydraLeftB2", "Hydra (L) B2"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftB3, LOCTEXT("HydraLeftB3", "Hydra (L) B3"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftB4, LOCTEXT("HydraLeftB4", "Hydra (L) B4"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftStart, LOCTEXT("HydraLeftStart", "Hydra (L) Start"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftTrigger, LOCTEXT("HydraLeftTrigger", "Hydra (L) Trigger"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftTriggerClick, LOCTEXT("HydraLeftTriggerClick", "Hydra (L) Trigger Click"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftBumper, LOCTEXT("HydraLeftBumper", "Hydra (L) Bumper"), FKeyDetails::GamepadKey));

		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftMotionX, LOCTEXT("HydraLeftMotionX", "Hydra (L) Motion X"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftMotionY, LOCTEXT("HydraLeftMotionY", "Hydra (L) Motion Y"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftMotionZ, LOCTEXT("HydraLeftMotionZ", "Hydra (L) Motion Z"), FKeyDetails::FloatAxis));

		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftRotationPitch, LOCTEXT("HydraLeftRotationPitch", "Hydra (L) Rotation Pitch"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftRotationYaw, LOCTEXT("HydraLeftRotationYaw", "Hydra (L) Rotation Yaw"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftRotationRoll, LOCTEXT("HydraLeftRotationRoll", "Hydra (L) Rotation Roll"), FKeyDetails::FloatAxis));

		//Right
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightJoystickX, LOCTEXT("HydraRightJoystickX", "Hydra (R) Joystick X"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightJoystickY, LOCTEXT("HydraRightJoystickY", "Hydra (R) Joystick Y"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightJoystickClick, LOCTEXT("HydraRightJoystickClick", "Hydra (R) Joystick Click"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightB1, LOCTEXT("HydraRightB1", "Hydra (R) B1"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightB2, LOCTEXT("HydraRightB2", "Hydra (R) B2"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightB3, LOCTEXT("HydraRightB3", "Hydra (R) B3"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightB4, LOCTEXT("HydraRightB4", "Hydra (R) B4"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightStart, LOCTEXT("HydraRightStart", "Hydra (R) Start"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightTrigger, LOCTEXT("HydraRightTrigger", "Hydra (R) Trigger"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightTriggerClick, LOCTEXT("HydraRightTriggerClick", "Hydra (R) Trigger Click"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightBumper, LOCTEXT("HydraRightBumper", "Hydra (R) Bumper"), FKeyDetails::GamepadKey));

		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightMotionX, LOCTEXT("HydraRightMotionX", "Hydra (R) Motion X"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightMotionY, LOCTEXT("HydraRightMotionY", "Hydra (R) Motion Y"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightMotionZ, LOCTEXT("HydraRightMotionZ", "Hydra (R) Motion Z"), FKeyDetails::FloatAxis));

		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightRotationPitch, LOCTEXT("HydraRightRotationPitch", "Hydra (R) Rotation Pitch"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightRotationYaw, LOCTEXT("HydraRightRotationYaw", "Hydra (R) Rotation Yaw"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightRotationRoll, LOCTEXT("HydraRightRotationRoll", "Hydra (R) Rotation Roll"), FKeyDetails::FloatAxis));

		//Add it according to the HydraEnum keys, NB: do not desync from the keys!
		/*
		HYDRA_BUTTON_B1,
		HYDRA_BUTTON_B2,
		HYDRA_BUTTON_B3,
		HYDRA_BUTTON_B4,
		HYDRA_BUTTON_START,
		HYDRA_BUTTON_JOYSTICK,
		HYDRA_BUTTON_BUMPER,
		HYDRA_BUTTON_TRIGGER
		*/

		LeftKeyMap.Empty();
		LeftKeyMap.Add(FHydraKeyMap(EKeysHydra::HydraLeftB1, FGamepadKeyNames::MotionController_Left_FaceButton1));
		LeftKeyMap.Add(FHydraKeyMap(EKeysHydra::HydraLeftB2, FGamepadKeyNames::MotionController_Left_FaceButton2));
		LeftKeyMap.Add(FHydraKeyMap(EKeysHydra::HydraLeftB3, FGamepadKeyNames::MotionController_Left_FaceButton3));
		LeftKeyMap.Add(FHydraKeyMap(EKeysHydra::HydraLeftB4, FGamepadKeyNames::MotionController_Left_FaceButton4));
		LeftKeyMap.Add(FHydraKeyMap(EKeysHydra::HydraLeftStart, FGamepadKeyNames::MotionController_Left_FaceButton5));
		LeftKeyMap.Add(FHydraKeyMap(EKeysHydra::HydraLeftJoystickClick, FGamepadKeyNames::MotionController_Left_FaceButton6));
		LeftKeyMap.Add(FHydraKeyMap(EKeysHydra::HydraLeftBumper, FGamepadKeyNames::MotionController_Left_Shoulder));
		LeftKeyMap.Add(FHydraKeyMap(EKeysHydra::HydraLeftTriggerClick, FGamepadKeyNames::MotionController_Left_Trigger));

		RightKeyMap.Empty();
		RightKeyMap.Add(FHydraKeyMap(EKeysHydra::HydraRightB1, FGamepadKeyNames::MotionController_Right_FaceButton1));
		RightKeyMap.Add(FHydraKeyMap(EKeysHydra::HydraRightB2, FGamepadKeyNames::MotionController_Right_FaceButton2));
		RightKeyMap.Add(FHydraKeyMap(EKeysHydra::HydraRightB3, FGamepadKeyNames::MotionController_Right_FaceButton3));
		RightKeyMap.Add(FHydraKeyMap(EKeysHydra::HydraRightB4, FGamepadKeyNames::MotionController_Right_FaceButton4));
		RightKeyMap.Add(FHydraKeyMap(EKeysHydra::HydraRightStart, FGamepadKeyNames::MotionController_Right_FaceButton5));
		RightKeyMap.Add(FHydraKeyMap(EKeysHydra::HydraRightJoystickClick, FGamepadKeyNames::MotionController_Right_FaceButton6));
		RightKeyMap.Add(FHydraKeyMap(EKeysHydra::HydraRightBumper, FGamepadKeyNames::MotionController_Right_Shoulder));
		RightKeyMap.Add(FHydraKeyMap(EKeysHydra::HydraRightTriggerClick, FGamepadKeyNames::MotionController_Right_Trigger));
	}

	//Init and Runtime
	FHydraController(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler)
		: MessageHandler(InMessageHandler), pluginPointer(nullptr)
	{
		UE_LOG(HydraPluginLog, Log, TEXT("Attempting to startup Hydra Module"));

		collector = new DataCollector;

		//This is a fixed relative path, meaning this file needs to exist for the plugin to work, even in shipping build!
		//Todo: fix path lookup when packaged
		//FString DllFilepath = FPaths::ConvertRelativePathToFull(FPaths::Combine(*FPaths::GameDir(),
		//	TEXT("Plugins"), TEXT("HydraPlugin"), TEXT("Binaries/Win64")), TEXT("sixense_x64.dll"));

		//Define Paths for direct dll bind
		FString BinariesRoot = FPaths::Combine(*FPaths::GameDir(), TEXT("Binaries"));
		FString PluginRoot = IPluginManager::Get().FindPlugin("HydraPlugin")->GetBaseDir();
		FString PlatformString;
		FString SixenseDLLString;

#if PLATFORM_WINDOWS
#if _WIN64
		//64bit
		SixenseDLLString = FString(TEXT("sixense_x64.dll"));
		PlatformString = FString(TEXT("Win64"));
#else
		//32bit
		SixenseDLLString = FString(TEXT("sixense.dll"));
		PlatformString = FString(TEXT("Win32"));
#endif
#else
		UE_LOG(LogClass, Error, TEXT("Unsupported Platform. Hydra Unavailable."));
#endif
		
		FString DllFilepath = FPaths::ConvertRelativePathToFull(FPaths::Combine(*PluginRoot, TEXT("Binaries"), *PlatformString, *SixenseDLLString));

		UE_LOG(HydraPluginLog, Log, TEXT("Fetching dll from %s"), *DllFilepath);

		//Check if the file exists, if not, give a detailed log entry why
		if (!FPaths::FileExists(DllFilepath)){
			UE_LOG(HydraPluginLog, Error, TEXT("%s File is missing (Did you copy Binaries into project root?)! Hydra Unavailable."), *SixenseDLLString);
			return;
		}

		DLLHandle = NULL;
		DLLHandle = FPlatformProcess::GetDllHandle(*DllFilepath);

		if (!DLLHandle){
			UE_LOG(HydraPluginLog, Error, TEXT("GetDllHandle failed, Hydra Unavailable."));
			UE_LOG(HydraPluginLog, Error, TEXT("Full path debug: %s."), *DllFilepath);
			return;
		}

		HydraInit = (dll_sixenseInit)FPlatformProcess::GetDllExport(DLLHandle, TEXT("sixenseInit"));
		HydraExit = (dll_sixenseExit)FPlatformProcess::GetDllExport(DLLHandle, TEXT("sixenseExit"));
		HydraGetAllNewestData = (dll_sixenseGetAllNewestData)FPlatformProcess::GetDllExport(DLLHandle, TEXT("sixenseGetAllNewestData"));

		collector->AllDataUE.available = (HydraInit() == SIXENSE_SUCCESS);

		if (collector->AllDataUE.available)
		{
			UE_LOG(HydraPluginLog, Log, TEXT("Hydra Available."));

			//Attach all EKeys
			AddInputMappingKeys();
		}
		else
		{
			UE_LOG(HydraPluginLog, Log, TEXT("Hydra Unavailable."));
		}

		//Required calls at init
		IModularFeatures::Get().RegisterModularFeature(GetModularFeatureName(), this);

		
	}

#undef LOCTEXT_NAMESPACE


	virtual ~FHydraController()
	{
		int cleanshutdown = HydraExit();

		FPlatformProcess::FreeDllHandle(DLLHandle);

		if (cleanshutdown == SIXENSE_SUCCESS)
		{
			UE_LOG(HydraPluginLog, Log, TEXT("Hydra Clean shutdown."));
		}

		delete collector;
	}

	virtual void Tick(float DeltaTime) override
	{

	}

	virtual void SendControllerEvents() override
	{
		//Use late sampling attached to SendControllerEvents
		HydraInputTick();
	}


	virtual ETrackingStatus GetControllerTrackingStatus(const int32 ControllerIndex, const EControllerHand DeviceHand) const
	{
		//Check if we're plugged in
		if (!pluginPointer->IsPluggedInAndEnabled())
		{
			return ETrackingStatus::NotTracked;
		}

		//Get data to see if we're tracked
		FHydraControllerData Controller;
		if (DeviceHand == EControllerHand::Left)
		{
			pluginPointer->LeftHandData(Controller);
		}
		else if (DeviceHand == EControllerHand::Right)
		{
			pluginPointer->RightHandData(Controller);
		}
		
		if (Controller.Enabled && !Controller.Docked)
		{
			return ETrackingStatus::Tracked;
		}
		else
		{
			return ETrackingStatus::NotTracked;
		}
	}

	//Not guaranteed to work atm
	float GetWorldScale() const
	{
		if (GEngine != nullptr && GEngine->GetWorld() != nullptr)
		{
			return GEngine->GetWorld()->GetWorldSettings()->WorldToMeters;
		}
		return 100.f;
	}

	//Hydra only supports one player so ControllerIndex is ignored.
	virtual bool GetControllerOrientationAndPosition(const int32 ControllerIndex, const EControllerHand DeviceHand, FRotator& OutOrientation, FVector& OutPosition) const
	{
		bool RetVal = false;
		
		FHydraControllerData Controller;
		if (DeviceHand == EControllerHand::Left)
		{
			RetVal = pluginPointer->LeftHandData(Controller);
		}
		else if (DeviceHand == EControllerHand::Right)
		{
			RetVal = pluginPointer->RightHandData(Controller);
		}

		if (!Controller.Enabled || Controller.Docked)
		{
			return false;
		}

		OutPosition = Controller.Position;
		OutOrientation = Controller.Orientation;

		return RetVal;
	}

	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override
	{
		return false;
	}

	void SetChannelValue(int32 UnrealControllerId, FForceFeedbackChannelType ChannelType, float Value) override
	{
	}
	void SetChannelValues(int32 UnrealControllerId, const FForceFeedbackValues& Values) override
	{
	}
	virtual void SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) override
	{
		MessageHandler = InMessageHandler;
	}

private:
	//Delegate Private functions
	void HydraInputTick();

	void BroadcastButtonChangeForController(EHydraControllerButton Button, const FHydraControllerData& Latest, bool ButtonPressed);
	void BroadcastButtonPressForController(EHydraControllerButton Button, const FHydraControllerData& Controller);
	void BroadcastButtonReleaseForController(EHydraControllerButton Button, const FHydraControllerData& Controller);
};


/** Delegate Functions, called by plugin to keep data in sync and to emit the events.*/

/** Internal Tick - Called by the Plugin */
void FHydraController::HydraInputTick()
{
	//Update to our latest data
	collector->GetLatestData(pluginPointer->GetCalibrationTransform().GetLocation());

	//run the loop and emit events
	SixenseAllControllerDataUE& LatestSet = collector->AllDataUE;
	SixenseAllControllerDataUE& PreviousSet = collector->HistoricalDataUE[1];

	//Did our enabled count change?
	if (LatestSet.enabledCount != PreviousSet.enabledCount)
	{
		//If we have a full enabled count we just plugged the hydra in
		if (LatestSet.isValidAndTracking())
		{
			pluginPointer->CallFunctionOnDelegates([&](UHydraControllerComponent* Component)
			{
				Component->OnPluggedIn.Broadcast();
			});
		}
		else
		{
			pluginPointer->CallFunctionOnDelegates([&](UHydraControllerComponent* Component)
			{
				Component->OnUnplugged.Broadcast();
			});
		}
	}

	//If we're not tracking that's it leave the loop early
	if (!LatestSet.isValidAndTracking())
	{
		return;
	}

	for (int i = 0; i < MAX_CONTROLLERS_SUPPORTED; i++)
	{
		FHydraControllerData Latest, Previous;

		//Convert to organized UE struct
		Latest.SetFromSixenseDataUE(LatestSet.controllers[i]);
		Previous.SetFromSixenseDataUE(PreviousSet.controllers[i]);

		//We don't care about disabled controllers
		if (!Latest.Enabled)
		{
			continue;
		}

		//Store for future hand comparisons
		bool leftHand = Latest.HandPossession == EHydraControllerHand::HYDRA_HAND_LEFT;

		//Docking change
		if (Latest.Docked != Previous.Docked)
		{
			if (Latest.Docked)
			{
				pluginPointer->CallFunctionOnDelegates([&](UHydraControllerComponent* Component)
				{
					Component->ControllerDocked.Broadcast(Latest);
				});
			}
			else
			{
				pluginPointer->CallFunctionOnDelegates([&](UHydraControllerComponent* Component)
				{
					Component->ControllerUndocked.Broadcast(Latest);
				});
			}
		}

		//Buttons

		//Trigger
		if (Latest.Trigger != Previous.Trigger)
		{
			//Input mapping
			if (leftHand)
			{
				EmitAnalogInputEventForKey(EKeysHydra::HydraLeftTrigger, Latest.Trigger, 0, 0);
				EmitAnalogInputEventForKey(FGamepadKeyNames::MotionController_Left_TriggerAxis, Latest.Trigger, 0, 0);
			}
			else
			{
				EmitAnalogInputEventForKey(EKeysHydra::HydraRightTrigger, Latest.Trigger, 0, 0);
				EmitAnalogInputEventForKey(FGamepadKeyNames::MotionController_Right_TriggerAxis, Latest.Trigger, 0, 0);
			}

			if (Latest.TriggerPressed != Previous.TriggerPressed)
			{
				BroadcastButtonChangeForController(HYDRA_BUTTON_TRIGGER, Latest, Latest.TriggerPressed);
			}
		}

		//Bumper
		if (Latest.BumperPressed != Previous.BumperPressed)
		{
			BroadcastButtonChangeForController(HYDRA_BUTTON_BUMPER, Latest, Latest.BumperPressed);
		}

		//B1
		if (Latest.B1Pressed != Previous.B1Pressed)
		{
			BroadcastButtonChangeForController(HYDRA_BUTTON_B1, Latest, Latest.B1Pressed);
		}

		//B2
		if (Latest.B2Pressed != Previous.B2Pressed)
		{
			BroadcastButtonChangeForController(HYDRA_BUTTON_B2, Latest, Latest.B2Pressed);
		}

		//B3
		if (Latest.B3Pressed != Previous.B3Pressed)
		{
			BroadcastButtonChangeForController(HYDRA_BUTTON_B3, Latest, Latest.B3Pressed);
		}

		//B4
		if (Latest.B4Pressed != Previous.B4Pressed)
		{
			BroadcastButtonChangeForController(HYDRA_BUTTON_B4, Latest, Latest.B4Pressed);
		}

		//Start
		if (Latest.StartPressed != Previous.StartPressed)
		{
			BroadcastButtonChangeForController(HYDRA_BUTTON_START, Latest, Latest.StartPressed);
		}

		//Joystick Click
		if (Latest.JoystickClicked != Previous.JoystickClicked)
		{
			BroadcastButtonChangeForController(HYDRA_BUTTON_JOYSTICK, Latest, Latest.JoystickClicked);
		}

		//Joystick movement
		if (Latest.Joystick.X != Previous.Joystick.X ||
			Latest.Joystick.Y != Previous.Joystick.Y)
		{
			if (leftHand)
			{
				EmitAnalogInputEventForKey(EKeysHydra::HydraLeftJoystickX, Latest.Joystick.X, 0, 0);
				EmitAnalogInputEventForKey(EKeysHydra::HydraLeftJoystickY, Latest.Joystick.Y, 0, 0);

				EmitAnalogInputEventForKey(FGamepadKeyNames::MotionController_Left_Thumbstick_X, Latest.Joystick.X, 0, 0);
				EmitAnalogInputEventForKey(FGamepadKeyNames::MotionController_Left_Thumbstick_Y, Latest.Joystick.Y, 0, 0);
			}
			else
			{
				EmitAnalogInputEventForKey(EKeysHydra::HydraRightJoystickX, Latest.Joystick.X, 0, 0);
				EmitAnalogInputEventForKey(EKeysHydra::HydraRightJoystickY, Latest.Joystick.Y, 0, 0);

				EmitAnalogInputEventForKey(FGamepadKeyNames::MotionController_Right_Thumbstick_X, Latest.Joystick.X, 0, 0);
				EmitAnalogInputEventForKey(FGamepadKeyNames::MotionController_Right_Thumbstick_Y, Latest.Joystick.Y, 0, 0);
			}
			pluginPointer->CallFunctionOnDelegates([&](UHydraControllerComponent* Component)
			{
				Component->JoystickMoved.Broadcast(Latest, Latest.Joystick);
			});
		}

		//Controller movement
		if (!Latest.Docked)
		{
			if (leftHand)
			{
				//2 meters = 1.0
				EmitAnalogInputEventForKey(EKeysHydra::HydraLeftMotionX, MotionInputMappingConversion(Latest.Position.X), 0, 0);
				EmitAnalogInputEventForKey(EKeysHydra::HydraLeftMotionY, MotionInputMappingConversion(Latest.Position.Y), 0, 0);
				EmitAnalogInputEventForKey(EKeysHydra::HydraLeftMotionZ, MotionInputMappingConversion(Latest.Position.Z), 0, 0);

				EmitAnalogInputEventForKey(EKeysHydra::HydraLeftRotationPitch, Latest.Orientation.Pitch / 90.f, 0, 0);
				EmitAnalogInputEventForKey(EKeysHydra::HydraLeftRotationYaw, Latest.Orientation.Yaw / 180.f, 0, 0);
				EmitAnalogInputEventForKey(EKeysHydra::HydraLeftRotationRoll, Latest.Orientation.Roll / 180.f, 0, 0);
			}
			else
			{
				//2 meters = 1.0
				EmitAnalogInputEventForKey(EKeysHydra::HydraRightMotionX, MotionInputMappingConversion(Latest.Position.X), 0, 0);
				EmitAnalogInputEventForKey(EKeysHydra::HydraRightMotionY, MotionInputMappingConversion(Latest.Position.Y), 0, 0);
				EmitAnalogInputEventForKey(EKeysHydra::HydraRightMotionZ, MotionInputMappingConversion(Latest.Position.Z), 0, 0);

				EmitAnalogInputEventForKey(EKeysHydra::HydraRightRotationPitch, Latest.Orientation.Pitch / 90.f, 0, 0);
				EmitAnalogInputEventForKey(EKeysHydra::HydraRightRotationYaw, Latest.Orientation.Yaw / 180.f, 0, 0);
				EmitAnalogInputEventForKey(EKeysHydra::HydraRightRotationRoll, Latest.Orientation.Roll / 180.f, 0, 0);
			}
			pluginPointer->CallFunctionOnDelegates([&](UHydraControllerComponent* Component)
			{
				Component->ControllerMoved.Broadcast(Latest, Latest.Position, Latest.Orientation);
			});
		}
	}
}

//convenience wrapper for emitting input mapping and delegate function calls
void FHydraController::BroadcastButtonChangeForController(EHydraControllerButton Button,
	const FHydraControllerData& Latest, 
	bool ButtonPressed)
{
	if (Latest.HandPossession == HYDRA_HAND_LEFT)
	{
		if (ButtonPressed)
		{
			EmitKeyDownEventForKey(LeftKeyMap[Button - 1].HydraKey, 0, 0);
			EmitKeyDownEventForKey(LeftKeyMap[Button - 1].MotionControllerKey, 0, 0);
			BroadcastButtonPressForController(Button, Latest);
		}
		else
		{
			EmitKeyUpEventForKey(LeftKeyMap[Button - 1].HydraKey, 0, 0);
			EmitKeyUpEventForKey(LeftKeyMap[Button - 1].MotionControllerKey, 0, 0);
			BroadcastButtonReleaseForController(Button, Latest);
		}
	}
	else
	{
		if (ButtonPressed)
		{
			EmitKeyDownEventForKey(RightKeyMap[Button - 1].HydraKey, 0, 0);
			EmitKeyDownEventForKey(RightKeyMap[Button - 1].MotionControllerKey, 0, 0);
			BroadcastButtonPressForController(Button, Latest);
		}
		else
		{
			EmitKeyUpEventForKey(RightKeyMap[Button - 1].HydraKey, 0, 0);
			EmitKeyUpEventForKey(RightKeyMap[Button - 1].MotionControllerKey, 0, 0);
			BroadcastButtonReleaseForController(Button, Latest);
		}
	}
}

void FHydraController::BroadcastButtonPressForController(EHydraControllerButton Button, const FHydraControllerData& Controller)
{
	pluginPointer->CallFunctionOnDelegates([&](UHydraControllerComponent* Component)
	{
		Component->ButtonPressed.Broadcast(Controller, Button);
	});
}

void FHydraController::BroadcastButtonReleaseForController(EHydraControllerButton Button, const FHydraControllerData& Controller)
{
	pluginPointer->CallFunctionOnDelegates([&](UHydraControllerComponent* Component)
	{
		Component->ButtonReleased.Broadcast(Controller, Button);
	});
}

#pragma endregion FHydraController

#pragma region FHydraPlugin - Methods

TSharedPtr< class IInputDevice > FHydraPlugin::CreateInputDevice(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler)
{
	controllerReference = new FHydraController(InMessageHandler);
	controllerReference->pluginPointer = this;
	inputDeviceCreated = true;

	return TSharedPtr< class IInputDevice >(controllerReference);
}

void FHydraPlugin::AddComponentDelegate(UHydraControllerComponent* DelegateComponent)
{
	//Only game component should receive callbacks
	UWorld* ComponentWorld = DelegateComponent->GetOwner()->GetWorld();
	if (ComponentWorld == nullptr)
	{
		return;
	}
	if (ComponentWorld->WorldType == EWorldType::Game ||
		ComponentWorld->WorldType == EWorldType::GamePreview ||
		ComponentWorld->WorldType == EWorldType::PIE)
	{
		ComponentDelegates.Add(DelegateComponent);
	}
}

void FHydraPlugin::RemoveComponentDelegate(UHydraControllerComponent* DelegateComponent)
{
	//Only game component should receive callbacks
	UWorld* ComponentWorld = DelegateComponent->GetOwner()->GetWorld();
	if (ComponentWorld == nullptr)
	{
		return;
	}
	if (ComponentWorld->WorldType == EWorldType::Game ||
		ComponentWorld->WorldType == EWorldType::GamePreview ||
		ComponentWorld->WorldType == EWorldType::PIE)
	{
		ComponentDelegates.Remove(DelegateComponent);
	}
}

void FHydraPlugin::SetCalibrationTransform(const FTransform& InCalibrationTransform)
{
	CalibrationTransform = InCalibrationTransform;
}

FTransform FHydraPlugin::GetCalibrationTransform()
{
	return CalibrationTransform;
}

bool FHydraPlugin::IsPluggedInAndEnabled()
{
	return controllerReference->collector->AllDataUE.isValidAndTracking();
}

bool FHydraPlugin::LeftHandData(FHydraControllerData& OutData)
{
	if (controllerReference->collector->LatestLeft != nullptr)
	{
		OutData.SetFromSixenseDataUE(*controllerReference->collector->LatestLeft);
		return true;
	}
	else
	{
		return false;
	}
}

bool FHydraPlugin::RightHandData(FHydraControllerData& OutData)
{
	if (controllerReference->collector->LatestRight != nullptr)
	{
		OutData.SetFromSixenseDataUE(*controllerReference->collector->LatestRight);
		return true;
	}
	else
	{
		return false;
	}
}

void FHydraPlugin::CallFunctionOnDelegates(TFunction< void(UHydraControllerComponent*)> InFunction)
{
	for (UHydraControllerComponent* ComponentDelegate : ComponentDelegates)
	{
		InFunction(ComponentDelegate);
	}
}

//Second parameter needs to be called the same as the Module name or packaging will fail
IMPLEMENT_MODULE(FHydraPlugin, HydraPlugin)	

#pragma endregion FHydraPlugin - Methods