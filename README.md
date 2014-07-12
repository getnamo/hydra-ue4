Overview


An Unofficial Razer Hydra Plugin for the Unreal Engine 4

The plugin is designed with an event driven architecture through a delegate interface. You can access device events through Blueprintable classes provided or through C++. Main C++ support is from inheriting the HydraDelegate, through it you can extend your own custom class to support Hydra events. Additional functions in the delegate support polling for latest data.

The plugin also handles hot plugging and emits HydraPluggedIn (HydraUnplugged for the reverse), allowing you to initialize if needed when the device is ready.

Main discussion thread 

## Quick Setup ##

 1.	Download 
 2.	Create new or choose project.
 3.	Browse to your project folder (typically found at Documents/Unreal Project/{Your Project Root})
 4.	Copy *Plugins* folder into your Project root.
 5.	Restart the Editor and open your project again.
 6.	Select Window->Plugins. Click on Installed and you should see a category called Input and a plugin called Hydra Plugin now available. Select Enabled. The Editor will warn you to restart, click restart.
 7.	When your project has reloaded, the plugin should be enabled and ready to use.
(Optional) You can confirm it has successfully loaded by opening the Class Viewer, searching "hydra" should show you one actor class added by the plugin calledHydraPluginActor.

### Events through Blueprint ###

 1.	Select Window->Class Viewer.
 2.	Search for "HydraPluginActor"
 3.	Right click the actor and Create a new Blueprint e.g. "HydraPluginActorBP"
 4.	Select Graph in the upper right hand corner and right click in the graph to bring up the function search
 5.	Typing in "hydra" will narrow the events down to plugin related.
 6.	Add your desired events and linkup to your desired functions
 7.	Add the created blueprint to the scene (it's an actor subclass) and hit Play.

e.g. If you want to get the position data from your Hydras add the Event Hydra Controller Moved. Right click again in an empty space in the BP graph and add a function call to Print String, connect position to string (a conversion node will automatically be made) and drag exec (the white triangle on node) from the event to the function to connect the calls.

Compile and Play to see the position data stream as printed output after you undock your hydra.

Optionally create a blueprint subclass from HydraPlayerControllerand assign it in your game mode. Functionality remains the same.

See tutorial video for a live example.

### Events through C++ ###

Use, embed, or subclass HydraPluginActor or HydraPlayerController and override functions you wish to subscribe to e.g.
```virtual void EventHydraTriggerPressed(int32 controller) OVERRIDE;```

Extend your own Class to Receive Events through C++

 1.	Include HydraDelegate.h and IHydraPlugin.h in your implementation
 2.	Ensure your project has "HydraPlugin" added to your PublicDependencyModuleNames in your *{Project}.build.cs*
 3.	Make your class inherit from HydraDelegate (multiple inheritence)
 4.	Copy HydraDelegate.cpp from the plugin source into your project source and add it your solution. Change top include to "your project name.h"
 5.	Add ```IHydraPlugin::Get().SetDelegate((HydraDelegate*)this);``` in your ```BeginPlay()``` or other initialization that is executed before the first tick.
 6.	Make your class tickable e.g. ```PrimaryActorTick.bCanEverTick = true;``` in your constructor.
 7.	Add ```IHydraPlugin::Get().HydraTick(DeltaTime); insideTick(float DeltaTime)```
 8.	Override any of the delegate methods to receive the events.

See tutorial video for a live example.

## Credit ##
Plugin made by Getnamo. Point all questions to the main discussion thread.
