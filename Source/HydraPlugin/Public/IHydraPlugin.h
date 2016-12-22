// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"
#include "HydraControllerComponent.h"
#include "IInputDeviceModule.h"

/**
 * The public interface to this module
 */
class HYDRAPLUGIN_API IHydraPlugin : public IInputDeviceModule
{

public:

	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static inline IHydraPlugin& Get()
	{
		return FModuleManager::LoadModuleChecked< IHydraPlugin >( "HydraPlugin" );
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded( "HydraPlugin" );
	}

	virtual void SetCalibrationTransform(const FTransform& CalibrationTransform) {};
	virtual FTransform GetCalibrationTransform() { return FTransform(); };
	virtual bool IsPluggedInAndEnabled() { return false; };

	virtual void AddComponentDelegate(UHydraControllerComponent* delegateComponent) {};
	virtual void RemoveComponentDelegate(UHydraControllerComponent* delegateComponent) {};

	virtual bool LeftHandData(FHydraControllerData& OutData) { return false; }
	virtual bool RightHandData(FHydraControllerData& OutData) { return false; }
};

