#include "HydraPluginPrivatePCH.h"

#include "IHydraPlugin.h"

#include "FHydraPlugin.h"
#include "HydraDelegate.h"

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

//Define each FKey const in a .cpp so we can compile
const FKey EKeysHydra::HydraLeftJoystickX("HydraLeftJoystickX");
const FKey EKeysHydra::HydraLeftJoystickY("HydraLeftJoystickY");
const FKey EKeysHydra::HydraLeftJoystickClick("HydraLeftJoystickClick");
const FKey EKeysHydra::HydraLeftB1("HydraLeftB1");
const FKey EKeysHydra::HydraLeftB2("HydraLeftB2");
const FKey EKeysHydra::HydraLeftB3("HydraLeftB3");
const FKey EKeysHydra::HydraLeftB4("HydraLeftB4");
const FKey EKeysHydra::HydraLeftStart("HydraLeftStart");
const FKey EKeysHydra::HydraLeftTrigger("HydraLeftTrigger");
const FKey EKeysHydra::HydraLeftTriggerClick("HydraLeftTriggerAxis");
const FKey EKeysHydra::HydraLeftBumper("HydraLeftBumper");

const FKey EKeysHydra::HydraLeftMotionX("HydraLeftMotionX");
const FKey EKeysHydra::HydraLeftMotionY("HydraLeftMotionY");
const FKey EKeysHydra::HydraLeftMotionZ("HydraLeftMotionZ");

const FKey EKeysHydra::HydraLeftRotationPitch("HydraLeftRotationPitch");
const FKey EKeysHydra::HydraLeftRotationYaw("HydraLeftRotationYaw");
const FKey EKeysHydra::HydraLeftRotationRoll("HydraLeftRotationRoll");

const FKey EKeysHydra::HydraRightJoystickX("HydraRightJoystickX");
const FKey EKeysHydra::HydraRightJoystickY("HydraRightJoystickY");
const FKey EKeysHydra::HydraRightJoystickClick("HydraRightJoystickClick");
const FKey EKeysHydra::HydraRightB1("HydraRightB1");
const FKey EKeysHydra::HydraRightB2("HydraRightB2");
const FKey EKeysHydra::HydraRightB3("HydraRightB3");
const FKey EKeysHydra::HydraRightB4("HydraRightB4");
const FKey EKeysHydra::HydraRightStart("HydraRightStart");
const FKey EKeysHydra::HydraRightTrigger("HydraRightTrigger");
const FKey EKeysHydra::HydraRightTriggerClick("HydraRightTriggerAxis");
const FKey EKeysHydra::HydraRightBumper("HydraRightBumper");

const FKey EKeysHydra::HydraRightMotionX("HydraRightMotionX");
const FKey EKeysHydra::HydraRightMotionY("HydraRightMotionY");
const FKey EKeysHydra::HydraRightMotionZ("HydraRightMotionZ");

const FKey EKeysHydra::HydraRightRotationPitch("HydraRightRotationPitch");
const FKey EKeysHydra::HydraRightRotationYaw("HydraRightRotationYaw");
const FKey EKeysHydra::HydraRightRotationRoll("HydraRightRotationRoll");


class DataCollector
{
public:
	DataCollector()
	{
		hydraDelegate = NULL;
		allData = new sixenseAllControllerData;
		allDataUE = new sixenseAllControllerDataUE;
	}
	~DataCollector()
	{
		delete allData;
		delete allDataUE;
	}

	sixenseControllerDataUE ConvertData(sixenseControllerData* data)
	{
		sixenseControllerDataUE converted;

		//Convert Sixense Axis to Unreal: UnrealX = - SixenseZ   UnrealY = SixenseX   UnrealZ = SixenseY
		converted.position = FVector(-data->pos[2]/10, data->pos[0]/10, data->pos[1]/10);	//converted
		converted.rotation = FQuat(data->rot_quat[2], -data->rot_quat[0], -data->rot_quat[1], data->rot_quat[3]);	//converted & rotation values inverted
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

		for (int i = 0; i < 4; i++)
		{
			allDataUE->controllers[i] = ConvertData(&allData->controllers[i]);
			if (allDataUE->controllers[i].enabled){
				allDataUE->enabledCount++;
			}
		}
	}

	sixenseAllControllerDataUE* allDataUE;
	sixenseAllControllerData* allData;
	HydraDelegate* hydraDelegate;
};

//Init and Runtime
void FHydraPlugin::StartupModule()
{
	UE_LOG(LogClass, Log, TEXT("Attempting to startup Hydra Module."));
	//try { //Removed because of C4530 (http://msdn.microsoft.com/en-us/library/2axwkyt4.aspx)
		collector = new DataCollector;

		FString DllFilename = FPaths::ConvertRelativePathToFull(FPaths::Combine(*FPaths::GameDir(),
			TEXT("Plugins"), TEXT("HydraPlugin"), TEXT("Binaries/Win64")), TEXT("sixense_x64.dll"));

		DLLHandle = NULL;
		DLLHandle = FPlatformProcess::GetDllHandle(*DllFilename);
		
		if (!DLLHandle){
			UE_LOG(LogClass, Error, TEXT("DLL missing, Hydra Unavailable."));
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
	collector->hydraDelegate = newDelegate;
	collector->hydraDelegate->HydraLatestData = collector->allDataUE;	//set the delegate latest pointer
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

	//update our delegate pointer to point to the freshest data (may be redundant but has to be called once)
	if (collector->hydraDelegate != NULL)
	{
		collector->hydraDelegate->HydraLatestData = collector->allDataUE;	//ensure the delegate.latest is always pointing to our converted data
		collector->hydraDelegate->InternalHydraControllerTick(DeltaTime);
	}
}