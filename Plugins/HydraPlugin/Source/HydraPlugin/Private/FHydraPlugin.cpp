#include "HydraPluginPrivatePCH.h"

#include "IHydraPlugin.h"

#include "FHydraPlugin.h"
#include "HydraDelegate.h"
#include "HydraSingleController.h"
#include "Slate.h"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <sixense.h>
#include <windows.h>

IMPLEMENT_MODULE(FHydraPlugin, HydraPlugin)

#define LOCTEXT_NAMESPACE "HydraPlugin"

//Private API - This is where the magic happens

//DLL import definition
typedef int (*dll_sixenseInit)(void);
typedef int (*dll_sixenseExit)(void);
typedef int (*dll_sixenseGetAllNewestData)(sixenseAllControllerData *);

dll_sixenseInit HydraInit;
dll_sixenseExit HydraExit;
dll_sixenseGetAllNewestData HydraGetAllNewestData;

//Collector class contains all the data captured from .dll and delegate data will point to this structure (allDataUE and historicalDataUE).
class DataCollector
{
public:
	DataCollector()
	{
		allData = new sixenseAllControllerData;
		allDataUE = new sixenseAllControllerDataUE;
		historicalDataUE = new sixenseAllControllerDataUE[10];
	}
	~DataCollector()
	{
		delete allData;
		delete allDataUE;
		delete[] historicalDataUE;
	}

	sixenseControllerDataUE ConvertData(sixenseControllerData* data)
	{
		sixenseControllerDataUE converted;

		//Convert Sixense Axis to Unreal: UnrealX = - SixenseZ   UnrealY = SixenseX   UnrealZ = SixenseY
		converted.position = FVector(-data->pos[2]/10, data->pos[0]/10, data->pos[1]/10);							//converted to cm from mm
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
		converted.enabled = (data->enabled != 0);
		converted.controller_index = data->controller_index;
		converted.is_docked = (data->is_docked != 0);
		converted.which_hand = data->which_hand;
		converted.hemi_tracking_enabled = (data->hemi_tracking_enabled != 0);

		return converted;
	}

	void ConvertAllData()
	{
		allDataUE->enabledCount = 0;

		for (int i = 0; i < MAX_CONTROLLERS_SUPPORTED; i++)
		{
			allDataUE->controllers[i] = ConvertData(&allData->controllers[i]);
			if (allDataUE->controllers[i].enabled){
				allDataUE->enabledCount++;
			}
		}
	}

	sixenseAllControllerDataUE* allDataUE;
	sixenseAllControllerData* allData;
	sixenseAllControllerDataUE* historicalDataUE;
};

//Init and Runtime
void FHydraPlugin::StartupModule()
{
	UE_LOG(LogClass, Log, TEXT("Attempting to startup Hydra Module."));
	//try { //Removed because of C4530 (http://msdn.microsoft.com/en-us/library/2axwkyt4.aspx)
		collector = new DataCollector;

		//This is a fixed relative path, meaning this file needs to exist for the plugin to work, even in shipping build!
		//Todo: automatically copy this file in the packaging process (how?).
		//FString DllFilepath = FPaths::ConvertRelativePathToFull(FPaths::Combine(*FPaths::GameDir(),
		//	TEXT("Plugins"), TEXT("HydraPlugin"), TEXT("Binaries/Win64")), TEXT("sixense_x64.dll"));

		//Define Paths for direct dll bind
		FString BinariesRoot = FPaths::Combine(*FPaths::GameDir(), TEXT("Binaries"));
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
		FString DllFilepath = FPaths::ConvertRelativePathToFull(FPaths::Combine(*BinariesRoot, *PlatformString, *SixenseDLLString));

		//Check if the file exists, if not, give a detailed log entry why
		if (!FPaths::FileExists(DllFilepath)){
			UE_LOG(LogClass, Error, TEXT("%s File is missing (Did you copy Binaries into project root?)! Hydra Unavailable."), *SixenseDLLString);
			return;
		}

		DLLHandle = NULL;
		DLLHandle = FPlatformProcess::GetDllHandle(*DllFilepath);
		
		if (!DLLHandle){
			UE_LOG(LogClass, Error, TEXT("GetDllHandle failed, Hydra Unavailable."));
			UE_LOG(LogClass, Error, TEXT("Full path debug: %s."), *DllFilepath);
			return;
		}

		HydraInit = (dll_sixenseInit)FPlatformProcess::GetDllExport(DLLHandle, TEXT("sixenseInit"));
		HydraExit = (dll_sixenseExit)FPlatformProcess::GetDllExport(DLLHandle, TEXT("sixenseExit"));
		HydraGetAllNewestData = (dll_sixenseGetAllNewestData)FPlatformProcess::GetDllExport(DLLHandle, TEXT("sixenseGetAllNewestData"));


		collector->allDataUE->available = (HydraInit() == SIXENSE_SUCCESS);

		if (collector->allDataUE->available)
		{
			UE_LOG(LogClass, Log, TEXT("Hydra Available."));

			//Attach all EKeys

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
		else
		{
			UE_LOG(LogClass, Log, TEXT("Hydra Unavailable."));
		}
	/*}
	catch (const std::exception& e) {
		UE_LOG(LogClass, Log, TEXT("Error: %s"),e.what());
	}*/
}

#undef LOCTEXT_NAMESPACE

void FHydraPlugin::ShutdownModule()
{
	int cleanshutdown = HydraExit();

	FPlatformProcess::FreeDllHandle(DLLHandle);

	if (cleanshutdown == SIXENSE_SUCCESS)
	{
		UE_LOG(LogClass, Log, TEXT("Hydra Clean shutdown."));
	}

	delete collector;
}


//Public API Implementation

/** Public API - Required **/

void FHydraPlugin::SetDelegate(HydraDelegate* newDelegate)
{
	hydraDelegate = newDelegate;
	hydraDelegate->HydraLatestData = collector->allDataUE;	//set the delegate latest data pointer
	hydraDelegate->HydraHistoryData = collector->historicalDataUE; //set the delegate history data pointer
}

void FHydraPlugin::HydraTick(float DeltaTime)
{
	//get the freshest data
	int success = HydraGetAllNewestData(collector->allData);
	if (success == SIXENSE_FAILURE){
		UE_LOG(LogClass, Error, TEXT("Hydra Error! Failed to get freshest data."));
		return;
	}
	//if the hydras are unavailable don't try to get more information
	if (!collector->allDataUE->available){
		UE_LOG(LogClass, Log, TEXT("Collector data not available."));
		return;
	}

	//convert and pass the data to the delegate
	collector->ConvertAllData();

	//Call the delegate once it has been set
	if (hydraDelegate != NULL)
	{
		DelegateTick(DeltaTime);
	}
}


/** Delegate Functions, called by plugin to keep data in sync and to emit the events.*/
void FHydraPlugin::DelegateUpdateAllData()
{
	hydraDelegate->HydraHistoryData[9] = hydraDelegate->HydraHistoryData[8];
	hydraDelegate->HydraHistoryData[8] = hydraDelegate->HydraHistoryData[7];
	hydraDelegate->HydraHistoryData[7] = hydraDelegate->HydraHistoryData[6];
	hydraDelegate->HydraHistoryData[6] = hydraDelegate->HydraHistoryData[5];
	hydraDelegate->HydraHistoryData[5] = hydraDelegate->HydraHistoryData[4];
	hydraDelegate->HydraHistoryData[4] = hydraDelegate->HydraHistoryData[3];
	hydraDelegate->HydraHistoryData[3] = hydraDelegate->HydraHistoryData[2];
	hydraDelegate->HydraHistoryData[2] = hydraDelegate->HydraHistoryData[1];
	hydraDelegate->HydraHistoryData[1] = hydraDelegate->HydraHistoryData[0];
	//NB: HydraHistoryData[0] = *HydraLatestData gets updated after the tick to take in integrated data
}

void FHydraPlugin::DelegateCheckEnabledCount(bool* plugNotChecked)
{
	if (!*plugNotChecked) return;

	sixenseAllControllerDataUE* previous = &hydraDelegate->HydraHistoryData[0];
	int32 oldCount = previous->enabledCount;
	int32 count = hydraDelegate->HydraLatestData->enabledCount;
	if (oldCount != count)
	{
		if (count == 2)	//hydra controller number, STEM behavior undefined.
		{
			hydraDelegate->HydraPluggedIn();
			*plugNotChecked = false;
		}
		else if (count == 0)
		{
			hydraDelegate->HydraUnplugged();
			*plugNotChecked = false;
		}
	}
}

//Function used for consistent conversion to input mapping basis
float MotionInputMappingConversion(float AxisValue){
	return FMath::Clamp(AxisValue / 200.f, -1.f, 1.f);
}

/** Internal Tick - Called by the Plugin */
void FHydraPlugin::DelegateTick(float DeltaTime)
{
	//Update Data History
	DelegateUpdateAllData();

	sixenseControllerDataUE* controller;
	sixenseControllerDataUE* previous;
	bool plugNotChecked = true;

	//Trigger any delegate events
	for (int i = 0; i < MAX_CONTROLLERS_SUPPORTED; i++)
	{

		controller = &hydraDelegate->HydraLatestData->controllers[i];
		previous = &hydraDelegate->HydraHistoryData[0].controllers[i];

		//Sync any extended data forms such as HydraSingleController, so that the correct instance values are sent and not 1 frame delayed
		hydraDelegate->UpdateControllerReference(controller, i);

		//If it is enabled run through all the event notifications and data integration
		if (controller->enabled)
		{
			//Enable Check
			if (controller->enabled != previous->enabled)
			{
				DelegateCheckEnabledCount(&plugNotChecked);
				hydraDelegate->HydraControllerEnabled(i);

				//Skip this loop, previous state comparisons will be wrong
				continue;
			}

			//Docking
			if (controller->is_docked != previous->is_docked)
			{
				if (controller->is_docked)
				{
					hydraDelegate->HydraDocked(i);
				}
				else{
					hydraDelegate->HydraUndocked(i);
				}
			}

			//Determine Hand to support dynamic input mapping
			bool leftHand = hydraDelegate->HydraWhichHand(i) == 1;

			//** Buttons */

			//Trigger
			if (controller->trigger < 0.5)
			{
				controller->trigger_pressed = false;
			}
			else{
				controller->trigger_pressed = true;
			}

			if (controller->trigger != previous->trigger)
			{
				hydraDelegate->HydraTriggerChanged(i, controller->trigger);
				//InputMapping
				if (leftHand)
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraLeftTrigger, 0, controller->trigger);
				else
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraRightTrigger, 0, controller->trigger);

				if (controller->trigger_pressed != previous->trigger_pressed)
				{

					if (controller->trigger_pressed)
					{
						hydraDelegate->HydraButtonPressed(i, HYDRA_BUTTON_TRIGGER);
						hydraDelegate->HydraTriggerPressed(i);
						//InputMapping
						if (leftHand)
							FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraLeftTriggerClick, 0, 0);
						else
							FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraRightTriggerClick, 0, 0);
					}
					else{
						hydraDelegate->HydraButtonReleased(i, HYDRA_BUTTON_TRIGGER);
						hydraDelegate->HydraTriggerReleased(i);
						//InputMapping
						if (leftHand)
							FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraLeftTriggerClick, 0, 0);
						else
							FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraRightTriggerClick, 0, 0);
					}
				}
			}

			//Bumper
			if ((controller->buttons & SIXENSE_BUTTON_BUMPER) != (previous->buttons & SIXENSE_BUTTON_BUMPER))
			{
				if ((controller->buttons & SIXENSE_BUTTON_BUMPER) == SIXENSE_BUTTON_BUMPER)
				{
					hydraDelegate->HydraButtonPressed(i, HYDRA_BUTTON_BUMPER);
					hydraDelegate->HydraBumperPressed(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraLeftBumper, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraRightBumper, 0, 0);
				}
				else{
					hydraDelegate->HydraButtonReleased(i, HYDRA_BUTTON_BUMPER);
					hydraDelegate->HydraBumperReleased(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraLeftBumper, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraRightBumper, 0, 0);
				}
			}

			//B1
			if ((controller->buttons & SIXENSE_BUTTON_1) != (previous->buttons & SIXENSE_BUTTON_1))
			{
				if ((controller->buttons & SIXENSE_BUTTON_1) == SIXENSE_BUTTON_1)
				{
					hydraDelegate->HydraButtonPressed(i, HYDRA_BUTTON_B1);
					hydraDelegate->HydraB1Pressed(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraLeftB1, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraRightB1, 0, 0);
				}
				else{
					hydraDelegate->HydraButtonReleased(i, HYDRA_BUTTON_B1);
					hydraDelegate->HydraB1Released(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraLeftB1, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraRightB1, 0, 0);
				}
			}
			//B2
			if ((controller->buttons & SIXENSE_BUTTON_2) != (previous->buttons & SIXENSE_BUTTON_2))
			{
				if ((controller->buttons & SIXENSE_BUTTON_2) == SIXENSE_BUTTON_2)
				{
					hydraDelegate->HydraButtonPressed(i, HYDRA_BUTTON_B2);
					hydraDelegate->HydraB2Pressed(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraLeftB2, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraRightB2, 0, 0);
				}
				else{
					hydraDelegate->HydraButtonReleased(i, HYDRA_BUTTON_B2);
					hydraDelegate->HydraB2Released(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraLeftB2, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraRightB2, 0, 0);
				}
			}
			//B3
			if ((controller->buttons & SIXENSE_BUTTON_3) != (previous->buttons & SIXENSE_BUTTON_3))
			{
				if ((controller->buttons & SIXENSE_BUTTON_3) == SIXENSE_BUTTON_3)
				{
					hydraDelegate->HydraButtonPressed(i, HYDRA_BUTTON_B3);
					hydraDelegate->HydraB3Pressed(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraLeftB3, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraRightB3, 0, 0);
				}
				else{
					hydraDelegate->HydraButtonReleased(i, HYDRA_BUTTON_B3);
					hydraDelegate->HydraB3Released(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraLeftB3, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraRightB3, 0, 0);
				}
			}
			//B4
			if ((controller->buttons & SIXENSE_BUTTON_4) != (previous->buttons & SIXENSE_BUTTON_4))
			{
				if ((controller->buttons & SIXENSE_BUTTON_4) == SIXENSE_BUTTON_4)
				{
					hydraDelegate->HydraButtonPressed(i, HYDRA_BUTTON_B4);
					hydraDelegate->HydraB4Pressed(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraLeftB4, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraRightB4, 0, 0);
				}
				else{
					hydraDelegate->HydraButtonReleased(i, HYDRA_BUTTON_B4);
					hydraDelegate->HydraB4Released(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraLeftB4, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraRightB4, 0, 0);
				}
			}

			//Start
			if ((controller->buttons & SIXENSE_BUTTON_START) != (previous->buttons & SIXENSE_BUTTON_START))
			{
				if ((controller->buttons & SIXENSE_BUTTON_START) == SIXENSE_BUTTON_START)
				{
					hydraDelegate->HydraButtonPressed(i, HYDRA_BUTTON_START);
					hydraDelegate->HydraStartPressed(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraLeftStart, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraRightStart, 0, 0);
				}
				else{
					hydraDelegate->HydraButtonReleased(i, HYDRA_BUTTON_START);
					hydraDelegate->HydraStartReleased(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraLeftStart, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraRightStart, 0, 0);
				}
			}

			//Joystick Click
			if ((controller->buttons & SIXENSE_BUTTON_JOYSTICK) != (previous->buttons & SIXENSE_BUTTON_JOYSTICK))
			{
				if ((controller->buttons & SIXENSE_BUTTON_JOYSTICK) == SIXENSE_BUTTON_JOYSTICK)
				{
					hydraDelegate->HydraButtonPressed(i, HYDRA_BUTTON_JOYSTICK);
					hydraDelegate->HydraJoystickPressed(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraLeftJoystickClick, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonPressed(EKeysHydra::HydraRightJoystickClick, 0, 0);
				}
				else{
					hydraDelegate->HydraButtonReleased(i, HYDRA_BUTTON_JOYSTICK);
					hydraDelegate->HydraJoystickReleased(i);
					//InputMapping
					if (leftHand)
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraLeftJoystickClick, 0, 0);
					else
						FSlateApplication::Get().OnControllerButtonReleased(EKeysHydra::HydraRightJoystickClick, 0, 0);
				}
			}

			/** Movement */

			//Joystick
			if (controller->joystick.X != previous->joystick.X ||
				controller->joystick.Y != previous->joystick.Y)
			{
				hydraDelegate->HydraJoystickMoved(i, controller->joystick);
				//InputMapping
				if (leftHand)
				{
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraLeftJoystickX, 0, controller->joystick.X);
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraLeftJoystickY, 0, controller->joystick.Y);
				}
				else
				{
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraRightJoystickX, 0, controller->joystick.X);
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraRightJoystickY, 0, controller->joystick.Y);
				}

				//axis test
				//FSlateApplication::Get().on
			}

			//Controller

			//Calculate Velocity, Acceleration, and angular velocity
			controller->velocity = (controller->position - previous->position) / DeltaTime;
			controller->acceleration = (controller->velocity - previous->velocity) / DeltaTime;
			controller->angular_velocity = FRotator(controller->quat - previous->quat);	//unscaled by deltatime
			float DeltaSquared = (DeltaTime*DeltaTime);
			controller->angular_velocity = FRotator(controller->angular_velocity.Pitch / DeltaSquared,
				controller->angular_velocity.Yaw / DeltaSquared,
				controller->angular_velocity.Roll / DeltaSquared);	//has to be scaled here to avoid clamping in the quaternion initialization

			if (!controller->is_docked){
				FRotator rotation = FRotator(controller->rotation);

				//If the controller isn't docked, it's moving
				hydraDelegate->HydraControllerMoved(i,
					controller->position, controller->velocity, controller->acceleration,
					rotation, controller->angular_velocity);

				//InputMapping
				if (leftHand)
				{
					//2 meters = 1.0
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraLeftMotionX, 0, MotionInputMappingConversion(controller->position.X));
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraLeftMotionY, 0, MotionInputMappingConversion(controller->position.Y));
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraLeftMotionZ, 0, MotionInputMappingConversion(controller->position.Z));
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraLeftRotationPitch, 0, rotation.Pitch / 90.f);
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraLeftRotationYaw, 0, rotation.Yaw / 180.f);
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraLeftRotationRoll, 0, rotation.Roll / 180.f);
				}
				else
				{
					//2 meters = 1.0
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraRightMotionX, 0, MotionInputMappingConversion(controller->position.X));
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraRightMotionY, 0, MotionInputMappingConversion(controller->position.Y));
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraRightMotionZ, 0, MotionInputMappingConversion(controller->position.Z));
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraRightRotationPitch, 0, rotation.Pitch / 90.f);
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraRightRotationYaw, 0, rotation.Yaw / 180.f);
					FSlateApplication::Get().OnControllerAnalog(EKeysHydra::HydraRightRotationRoll, 0, rotation.Roll / 180.f);
				}
			}
		}//end enabled
		else{
			if (controller->enabled != previous->enabled)
			{
				DelegateCheckEnabledCount(&plugNotChecked);
				hydraDelegate->HydraControllerDisabled(i);
			}
		}
	}//end controller for loop

	//Update the stored data with the integrated data obtained from latest
	hydraDelegate->HydraHistoryData[0] = *hydraDelegate->HydraLatestData;
}