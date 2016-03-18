## VR Editor Branch Instructions

Instructions:

 1.    Clone the [VREditor branch](https://github.com/EpicGames/UnrealEngine/tree/dev-vr-editor)
 2.	[Download VREditor hydra plugin release](https://github.com/getnamo/hydra-ue4/releases)
 3.	Browse to your cloned [engine root folder](https://github.com/EpicGames/UnrealEngine/tree/dev-vr-editor). Drag and drop *Engine* which contains the plugin there.
 4.    Compile and run the editor!
 5.    If you get VR Preview greyed out: What worked for me was to use the GenerateProjectFiles.bat after I put the HydraPlugin in and compiling the engine again. Next time I launched it, preview wasn't greyed out and VR editor worked.

![IMG](http://i.imgur.com/SRxh0p8.gif)

### Run VR Editor

![IMG](http://i.imgur.com/aIMygUi.png)

To calibrate your hydra offset drag and drop *HydraCalibrateActor* into the scene hit play and press *Hydra Bumper* and *Hydra Start* simultaneously on either hydra controller while making a T-pose.

[Example Gifv](http://i.imgur.com/fsa937B.gifv)

Press VR button to enter VR editor mode in a project of your choice.

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
