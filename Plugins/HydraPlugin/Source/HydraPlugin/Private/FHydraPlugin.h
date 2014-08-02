#pragma once

class DataCollector;
class HydraDelegate;

struct EKeysHydra
{
	static const FKey HydraLeftJoystickX;
	static const FKey HydraLeftJoystickY;
	static const FKey HydraLeftJoystickClick;
	static const FKey HydraLeftB1;
	static const FKey HydraLeftB2;
	static const FKey HydraLeftB3;
	static const FKey HydraLeftB4;
	static const FKey HydraLeftStart;
	static const FKey HydraLeftTrigger;
	static const FKey HydraLeftTriggerClick;
	static const FKey HydraLeftBumper;

	//NB capped to 1.0 at 2 meters from base
	static const FKey HydraLeftMotionX;
	static const FKey HydraLeftMotionY;
	static const FKey HydraLeftMotionZ;

	static const FKey HydraLeftRotationPitch;
	static const FKey HydraLeftRotationYaw;
	static const FKey HydraLeftRotationRoll;

	static const FKey HydraRightJoystickX;
	static const FKey HydraRightJoystickY;
	static const FKey HydraRightJoystickClick;
	static const FKey HydraRightB1;
	static const FKey HydraRightB2;
	static const FKey HydraRightB3;
	static const FKey HydraRightB4;
	static const FKey HydraRightStart;
	static const FKey HydraRightTrigger;
	static const FKey HydraRightTriggerClick;
	static const FKey HydraRightBumper;

	//NB capped to 1.0 at 2 meters from base
	static const FKey HydraRightMotionX;
	static const FKey HydraRightMotionY;
	static const FKey HydraRightMotionZ;

	static const FKey HydraRightRotationPitch;
	static const FKey HydraRightRotationYaw;
	static const FKey HydraRightRotationRoll;
};

class FHydraPlugin : public IHydraPlugin
{
public:
	/** IModuleInterface implementation */
	void StartupModule();
	void ShutdownModule();

	/** To subscribe to event calls, only supports one listener for now */
	void SetDelegate(HydraDelegate* newDelegate);

	/** Call this in your class Tick to update information */
	void HydraTick(float DeltaTime);

private:
	DataCollector *collector;
	HydraDelegate* hydraDelegate;
	void* DLLHandle;

	//Delegate Private functions
	void DelegateTick(float DeltaTime);
	void DelegateUpdateAllData();
	void DelegateCheckEnabledCount(bool* plugNotChecked);
};