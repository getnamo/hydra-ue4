#include "HydraPluginPrivatePCH.h"

#include "IHydraPlugin.h"
#include "IMotionController.h"


#include "SlateBasics.h"
#include "IPluginManager.h"
#include "HydraComponent.h"
#include "HydraUtility.h"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <sixense.h>
#include <windows.h>

#define LOCTEXT_NAMESPACE "HydraPlugin"
#define PLUGIN_VERSION "0.9.0"
#define HYDRA_HISTORY_MAX 10	//10 frame history for data
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

#pragma region DataCollector

//Collector class contains all the data captured from .dll and delegate data will point to this structure (allDataUE and historicalDataUE).
class DataCollector
{
public:
	DataCollector()
	{
	}
	~DataCollector()
	{
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
		allDataUE.enabledCount = 0;

		for (int i = 0; i < MAX_CONTROLLERS_SUPPORTED; i++)
		{
			allDataUE.controllers[i] = ConvertData(&allData.controllers[i], offset);
			if (allDataUE.controllers[i].enabled){
				allDataUE.enabledCount++;
			}
		}
	}

	void GetLatestData()
	{
		int success = HydraGetAllNewestData(&allData);
		if (success == SIXENSE_FAILURE) {
			UE_LOG(HydraPluginLog, Error, TEXT("Hydra Error! Failed to get freshest data."));
			return;
		}
		//if the hydras are unavailable don't try to get more information
		if (!allDataUE.available) {
			UE_LOG(HydraPluginLog, Log, TEXT("Collector data not available."));
			return;
		}

		ConvertAllData(IHydraPlugin::Get().GetCalibrationTransform()->GetLocation());

		//Set all historical data
		for (int i = 0; i < HYDRA_HISTORY_MAX-1; i++)
		{
			historicalDataUE[i+1] = historicalDataUE[i];
		}

		//latest
		historicalDataUE[0] = allDataUE;
	}

	sixenseAllControllerDataUE allDataUE;
	sixenseAllControllerData allData;
	sixenseAllControllerDataUE historicalDataUE[HYDRA_HISTORY_MAX];
};

#pragma endregion DataCollector

#pragma region FHydraController

class FHydraController : public IInputDevice, public IMotionController
{	

public:
	DataCollector *collector;

	void* DLLHandle;
	HydraUtilityTimer UtilityTimer;

	/** handler to send all messages to */
	TSharedRef<FGenericApplicationMessageHandler> MessageHandler;
	
	void AddInputMappingKeys()
	{
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftDocked, LOCTEXT("HydraLeftDocked", "Hydra Left Docked"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightDocked, LOCTEXT("HydraRightDocked", "Hydra Right Docked"), FKeyDetails::GamepadKey));

		//Left
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftJoystickX, LOCTEXT("HydraLeftJoystickX", "Hydra Left Joystick X"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftJoystickY, LOCTEXT("HydraLeftJoystickY", "Hydra Left Joystick Y"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftJoystickClick, LOCTEXT("HydraLeftJoystickClick", "Hydra Left Joystick Click"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftB1, LOCTEXT("HydraLeftB1", "Hydra Left B1"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftB2, LOCTEXT("HydraLeftB2", "Hydra Left B2"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftB3, LOCTEXT("HydraLeftB3", "Hydra Left B3"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftB4, LOCTEXT("HydraLeftB4", "Hydra Left B4"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftStart, LOCTEXT("HydraLeftStart", "Hydra Left Start"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftTrigger, LOCTEXT("HydraLeftTrigger", "Hydra Left Trigger"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftTriggerClick, LOCTEXT("HydraLeftTriggerClick", "Hydra Left Trigger Click"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftBumper, LOCTEXT("HydraLeftBumper", "Hydra Left Bumper"), FKeyDetails::GamepadKey));

		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftMotionX, LOCTEXT("HydraLeftMotionX", "Hydra Left Motion X"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftMotionY, LOCTEXT("HydraLeftMotionY", "Hydra Left Motion Y"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftMotionZ, LOCTEXT("HydraLeftMotionZ", "Hydra Left Motion Z"), FKeyDetails::FloatAxis));

		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftRotationPitch, LOCTEXT("HydraLeftRotationPitch", "Hydra Left Rotation Pitch"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftRotationYaw, LOCTEXT("HydraLeftRotationYaw", "Hydra Left Rotation Yaw"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraLeftRotationRoll, LOCTEXT("HydraLeftRotationRoll", "Hydra Left Rotation Roll"), FKeyDetails::FloatAxis));

		//Right
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightJoystickX, LOCTEXT("HydraRightJoystickX", "Hydra Right Joystick X"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightJoystickY, LOCTEXT("HydraRightJoystickY", "Hydra Right Joystick Y"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightJoystickClick, LOCTEXT("HydraRightJoystickClick", "Hydra Right Joystick Click"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightB1, LOCTEXT("HydraRightB1", "Hydra Right B1"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightB2, LOCTEXT("HydraRightB2", "Hydra Right B2"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightB3, LOCTEXT("HydraRightB3", "Hydra Right B3"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightB4, LOCTEXT("HydraRightB4", "Hydra Right B4"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightStart, LOCTEXT("HydraRightStart", "Hydra Right Start"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightTrigger, LOCTEXT("HydraRightTrigger", "Hydra Right Trigger"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightTriggerClick, LOCTEXT("HydraRightTriggerClick", "Hydra Right Trigger Click"), FKeyDetails::GamepadKey));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightBumper, LOCTEXT("HydraRightBumper", "Hydra Right Bumper"), FKeyDetails::GamepadKey));

		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightMotionX, LOCTEXT("HydraRightMotionX", "Hydra Right Motion X"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightMotionY, LOCTEXT("HydraRightMotionY", "Hydra Right Motion Y"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightMotionZ, LOCTEXT("HydraRightMotionZ", "Hydra Right Motion Z"), FKeyDetails::FloatAxis));

		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightRotationPitch, LOCTEXT("HydraRightRotationPitch", "Hydra Right Rotation Pitch"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightRotationYaw, LOCTEXT("HydraRightRotationYaw", "Hydra Right Rotation Yaw"), FKeyDetails::FloatAxis));
		EKeys::AddKey(FKeyDetails(EKeysHydra::HydraRightRotationRoll, LOCTEXT("HydraRightRotationRoll", "Hydra Right Rotation Roll"), FKeyDetails::FloatAxis));

	}

	//Init and Runtime
	FHydraController(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler)
		: MessageHandler(InMessageHandler)
	{
		UE_LOG(HydraPluginLog, Log, TEXT("Attempting to startup Hydra Module, v%s"), TEXT(PLUGIN_VERSION));

		collector = new DataCollector;
		//hydraDelegate->HydraHistoryData// = collector->historicalDataUE;

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

		collector->allDataUE.available = (HydraInit() == SIXENSE_SUCCESS);

		if (collector->allDataUE.available)
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
		/*if (!hydraDelegate->HydraIsAvailable())
		{
			return ETrackingStatus::NotTracked;
		}*/

		/*UHydraSingleController* controller = hydraDelegate->HydraControllerForControllerHand(DeviceHand);

		if (controller != nullptr && controller->enabled && !controller->docked)
		{
			return ETrackingStatus::Tracked;
		}
		else*/
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
		
		/*UHydraSingleController* controller = hydraDelegate->HydraControllerForControllerHand(DeviceHand);

		if (controller != nullptr && controller->enabled && !controller->docked)
		{
			OutOrientation = controller->orientation;
			OutPosition = controller->position  * (GetWorldScale() / 100.f);
			RetVal = true;
		}*/

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
};


/** Delegate Functions, called by plugin to keep data in sync and to emit the events.*/

/** Internal Tick - Called by the Plugin */
void FHydraController::HydraInputTick()
{
	//Update to our latest data
	collector->GetLatestData();

	//run the loop and emit events
	const sixenseAllControllerDataUE& Latest = collector->allDataUE;
	const sixenseAllControllerDataUE& Previous = collector->historicalDataUE[1];
}

#pragma endregion FHydraController

#pragma region FHydraPlugin

//Implementing the module, required
class FHydraPlugin : public IHydraPlugin
{
	FTransform CalibrationTransform;
	FHydraController* controllerReference = nullptr;
	TArray<UHydraPluginComponent*> ComponentDelegates;
	bool inputDeviceCreated = false;

public:

	virtual TSharedPtr< class IInputDevice > CreateInputDevice(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) override
	{
		controllerReference = new FHydraController(InMessageHandler);
		inputDeviceCreated = true;

		return TSharedPtr< class IInputDevice >(controllerReference);
	}

	virtual void AddComponentDelegate(UHydraPluginComponent* delegateComponent) override
	{
		ComponentDelegates.Add(delegateComponent);
	}

	virtual void RemoveComponentDelegate(UHydraPluginComponent* delegateComponent) override
	{
		ComponentDelegates.Remove(delegateComponent);
	}

	virtual void SetCalibrationTransform(const FTransform& InCalibrationTransform) override
	{
		CalibrationTransform = InCalibrationTransform;
	}

	virtual FTransform* GetCalibrationTransform() override
	{
		return &CalibrationTransform;
	}

	//Call this function on all the attached plugin component delegates
	void CallFunctionOnDelegates(TFunction< void(UHydraPluginComponent*)> InFunction)
	{
		for (UHydraPluginComponent* ComponentDelegate : ComponentDelegates)
		{
			InFunction(ComponentDelegate);
		}
	}


};

//Second parameter needs to be called the same as the Module name or packaging will fail
IMPLEMENT_MODULE(FHydraPlugin, HydraPlugin)	

#pragma endregion FHydraPlugin