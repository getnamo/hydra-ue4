These files are only relevant to the Packaged a.k.a Shipping build. If you're using the editor, ignore this folder.


When you're ready to ship:
1. As of UE4.3, your project needs code. Even if you're using blueprint only, add code to your project and compile (it doesn't need to do anything).
2. Package your game
3. Copy the 'Plugins' folder in here into your packaged build 'ProjectName' folder. E.g. if I packaged a project called 'HydraTest' in my packaged directory (typically called WindowsNoEditor) find the HydraTest folder and place the Plugins folder there.
4. Confirm its working by launching your packaged game from the 'Binaries' subfolder where you placed your 'Plugins' folder.