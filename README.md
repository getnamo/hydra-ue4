## Overview ##

An Unofficial Razer Hydra Plugin for the Unreal Engine 4

The plugin is designed with an event driven architecture through a delegate interface. You can access device events through Blueprintable classes provided or through C++. Main C++ support is from inheriting the HydraDelegate, through it you can extend your own custom class to support Hydra events. Additional functions in the delegate support polling for latest data.

The plugin also handles hot plugging and emits HydraPluggedIn (HydraUnplugged for the reverse), allowing you to initialize if needed when the device is ready.

[Main discussion thread](https://forums.unrealengine.com/showthread.php?3505-Razer-Hydra-Plugin)

[Unreal Engine Wiki](https://wiki.unrealengine.com/Unofficial_Hydra_Plugin)

## Quick Install & Setup ##

 1.	[Download Latest Release](https://github.com/getnamo/hydra-ue4/releases)
 2.	Create new or choose project.
 3.	Browse to your project folder (typically found at Documents/Unreal Project/{Your Project Root})
 4.	Copy *Plugins* folder into your Project root.
 5.	Restart the Editor and open your project again. Plugin is now ready to use.

## Motion Controller Documentation ##

Please refer to the [Motion Controller Component Setup unreal documentation](https://docs.unrealengine.com/latest/INT/Platforms/VR/MotionController/index.html) as this setup is hardware agnostic.

NB:
For input mapping purposes
Hydra start == Face Button 5
Hydra Joystick Click' == Face Button 6

## Hydra Specific and Calibration ##

![IMG](http://i.imgur.com/tTiZ3NI.png)

If you want to calibrate or do anything specific, just add a hydra plugin component (renamed to avoid confusion with earlier components). These components are multicast and their events are as well so place them *anywhere* in multiples if you want to. They are also where you would poll for current or historical data for gesture recognition/etc.

To calibrate you simply call Calibrate or SetBaseOffset on your component.

![IMG](http://i.imgur.com/BAyjapm.png)

Calibrate recalibrates the origin as coming from the centroid of your two hydras + the offset. In its default settings it should work as a good T-pose to shoulder midpoint.

![IMG](http://i.imgur.com/y82yzEv.png)

If you want to do your own calibration, just come up with a good position offset and set it here, all values onward will maintain this calibration, even between PIE sessions.

You can also set links to the MeshComponents attached to your MotionControllerComponents and let the plugin auto-hide the meshes whenever you dock that controller.

![IMG](http://i.imgur.com/coPNZTx.png)

Just call SetMeshComponentLinks at some initialization stage e.g. BeginPlay and then check the option to HideMeshComponentsWhenDocked



How about events? Now there are no more interfaces to deal with, instead you click on your hydra plugin component and click the green + to add the event from the details pane. This should feel familiar as these are the same blueprint assignable multicast delegates that PrimitiveComponents use for collision checks (e.g. OnBeginOverlap)

![IMG](http://i.imgur.com/YpEA2da.png)

e.g. if you want to have all the data to compare buttons or maybe do something with acceleration you can subscribe to the ControllerMoved event

![IMG](http://i.imgur.com/2MyKAHq.png)

To wrap it up, use input mapping events for all joystick and button input. You can use either the hardware agnostic motion controller buttons or the hydra specific ones, they work the same

![IMG](http://i.imgur.com/DvQDz0E.png)

## Shipping ##

When you're ready to ship, just package the folder. With the current state the dll gets copied, but the project may not launch (fatal error)

### Shipping Troubleshoot ###

You run your packaged project and you get the following errors

**Error:**

![alt text](http://i.imgur.com/IEIk7Rm.png "No Code Project Error")

Your project runtime also continues working, but your hydra does not respond.

**Fix:** This means that you have no code added to your project, the project needs code to run a plugin. Add any code (e.g. new pawn that doesn't do anything extra) and compile to fix.

**Error:**

![alt text](http://i.imgur.com/j4UAp8t.png "DLL not found Error")

Also you search your log file and find 
![alt text](http://i.imgur.com/jy6nsmX.png "Log of DLL not found Error")

**Fix:** This error means the sixense dll file is missing.

## Credit and License##
Plugin made by Getnamo. Point all questions to the main discussion thread.

The Sixense SDK is licensed under [Sixense EULA](http://sixense.com/eula).

<br />
Anything not covered by Sixense EULA and [Unreal Engine EULA](https://www.unrealengine.com/eula) in <span property="dct:title">Hydra UE4 Plugin</span> provided under [MIT license](https://en.wikipedia.org/wiki/MIT_License). Do anything you want just retain the notice.
