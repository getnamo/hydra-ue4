## Overview ##

An Unofficial [Razer Hydra](https://en.wikipedia.org/wiki/Razer_Hydra) plugin for the [Unreal Engine 4](https://www.unrealengine.com/). 

Since version 0.8, the plugin works through both Unreal Engine [Motion Controller](https://docs.unrealengine.com/latest/INT/Platforms/VR/MotionController/) and [Input Mapping](https://docs.unrealengine.com/latest/INT/Gameplay/Input/) abstraction as well as through a convenience component for hydra specific events and functions.

[Main discussion thread](https://forums.unrealengine.com/showthread.php?3505-Razer-Hydra-Plugin)

[Unreal Engine Wiki](https://wiki.unrealengine.com/Unofficial_Hydra_Plugin)

## Quick Install & Setup ##

 1.	[Download Latest Release](https://github.com/getnamo/hydra-ue4/releases)
 2.	Create new or choose project.
 3.	Browse to your project folder (typically found at Documents/Unreal Project/*{Your Project Root}*)
 4.	Copy *Plugins* folder into your Project root.
 5.    Enable the plugin via Edit->Plugins. Scroll down to Project->Input Devices. Click Enabled.
 5.	Restart the Editor and open your project again. Plugin is now ready to use.

## Motion Controller Documentation ##

Please refer to the [Motion Controller Component Setup unreal documentation](https://docs.unrealengine.com/latest/INT/Platforms/VR/MotionController/index.html) as this setup is hardware agnostic. Since 0.9 a basic motion controller setup should look like this:

![IMG](http://i.imgur.com/vbrj4wY.png)

Where visible static meshes are parented to the motion controllers which are in turn parented to a chosen offset, forward by 40cm from the camera in this particular example.

###Input Mapping

You can use either the Motion Controller input mapping (IM) events

![IMG](http://i.imgur.com/toBD86X.png)

or hydra specific ones

![IMG](http://i.imgur.com/4Y2QyLh.png)

###Hydra Tracking Notes

No hydra specific component is required, tracking is handled internally and passed to the agnostic motion controller components and the input mapping system. Tracking should defer to other motion controllers if unplugged or docked.

The origin of tracking is the hydra base station.

For input mapping purposes
Hydra start == Face Button 5
Hydra Joystick Click == Face Button 6

## Hydra Custom Functions and Events##

![IMG](http://i.imgur.com/wlpdujf.png)

If you want to calibrate or do complex hydra specific things, just add a hydra controller component. 

Component are multicast and their events are as well so place them *anywhere* in multiples if you want to. They are also where you would poll for latest data for gesture recognition/etc.

###Events
How about events? Now there are no more interfaces to deal with, instead you click on your hydra plugin component and click the green + to add the event from the details pane. This should feel familiar as these are the same blueprint assignable multicast delegates that PrimitiveComponents use for collision checks (e.g. OnBeginOverlap)

####On Controller Moved

E.g. draw a debug box for each hydra from a base offset scene component
![IMG](http://i.imgur.com/2IMOcTR.png)

####Button Pressed/Released

e.g. verbose log of button presses/releases. Notice that you can break additional information from the controller data structure.

![IMG](http://i.imgur.com/g9OUtM9.png)


####Joystick Moved

e.g. a verbose log of joystick movement


![IMG](http://i.imgur.com/eDDlKqZ.png)

####Hotplugging Events

Called when you plug in or unplug the controller from the system usb.
![IMG](http://i.imgur.com/jTfeerB.png)

the same type of events exist for docking.

### Optional Calibration methods
Used to calculated 1:1 offset for your controllers. To calibrate you simply call Calibrate or SetBaseOffset on your component.

![IMG](http://i.imgur.com/BAyjapm.png)

Calibrate recalibrates the origin as coming from the centroid of your two hydras + the offset. In its default settings it should work as a good T-pose to shoulder midpoint.

If you want to do your own calibration, just come up with a good position offset and set it here, all values onward will maintain this calibration, even between PIE sessions.

## Shipping ##

When you're ready to ship, just package the game it should all work out of the box.

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
