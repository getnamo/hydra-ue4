## Overview ##

An Unofficial Razer Hydra Plugin for the Unreal Engine 4

The plugin is designed with an event driven architecture through a delegate interface. You can access device events through Blueprintable classes provided or through C++. Main C++ support is from inheriting the HydraDelegate, through it you can extend your own custom class to support Hydra events. Additional functions in the delegate support polling for latest data.

The plugin also handles hot plugging and emits HydraPluggedIn (HydraUnplugged for the reverse), allowing you to initialize if needed when the device is ready.

[Main discussion thread](https://forums.unrealengine.com/showthread.php?3505-Razer-Hydra-Plugin)

[Unreal Engine Wiki](https://wiki.unrealengine.com/Unofficial_Hydra_Plugin)

## Quick Install & Setup ##

 1.	Download
 2.	Create new or choose project.
 3.	Browse to your project folder (typically found at Documents/Unreal Project/{Your Project Root})
 4.	Copy *Plugins* and *Binaries* folders into your Project root.
 5.	Restart the Editor and open your project again. Plugin is now ready to use.

## Motion Controller Documentation ##

Please refer to the [Motion Controller Component Setup unreal documentation](https://docs.unrealengine.com/latest/INT/Platforms/VR/MotionController/index.html) as this setup is hardware agnostic.

NB:
For input mapping purposes
Hydra start == Face Button 5
Hydra Joystick Click' == Face Button 6

Also you cannot calibrate the motion controller innately, add an offset to the component manually for now.

## Shipping ##

When you're ready to ship

<ol>
<li> Projects require code for shipping in order to include the plugin, if you are using a blueprint only project, add an empty class and compile your project module. You simply do File->Add Code to Project and it can be anything so I usually just pick None->Create Class and then it will ask you to open visual studio where you just hit compile (Build solution). If you haven't added code before follow the unreal engine <a href="https://docs.unrealengine.com/latest/INT/Programming/QuickStart/1/index.html">programming Quick Start</a> guide. Essentially it boils down to downloading the free Visual Studio Community and changing a few small configs.</li>
<li>Package your game
<li>Copy only the *Binaries* folder into your packaged build *{ProjectName}* folder. E.g. if I packaged a project called *HydraTest* in my packaged directory (typically called *WindowsNoEditor*) find the *HydraTest* folder and place the Plugins folder there.
<li>Confirm that it's working by launching your packaged game executable from the *Binaries* subfolder.
</ol>

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

**Fix:** This error means the sixense dll file is missing. Copy the *Binaries* folder from the plugin into your *{packaged root}/{Project Name}*

## Credit and License##
Plugin made by Getnamo. Point all questions to the main discussion thread.

The Sixense SDK is licensed under [Sixense EULA](http://sixense.com/eula).

<a rel="license" href="http://creativecommons.org/publicdomain/mark/1.0/">
<img src="http://i.creativecommons.org/p/mark/1.0/88x31.png"
     style="border-style: none;" alt="Public Domain Mark" />
</a>
<br />
Anything not covered by Sixense EULA and [Unreal Engine EULA](https://www.unrealengine.com/eula) in <span property="dct:title">Hydra UE4 Plugin</span> is free of known copyright restrictions.
