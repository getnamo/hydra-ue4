// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
    using System.IO;

	public class HydraPlugin : ModuleRules
	{
        private string ModulePath
        {
            get { return ModuleDirectory; }
        }

        private string ThirdPartyPath
        {
            get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
        }

        private string BinariesPath
        {
            get { return Path.GetFullPath(Path.Combine(ModulePath, "../../Binaries/")); }
        }

		public HydraPlugin(TargetInfo Target)
		{
			PublicIncludePaths.AddRange(
				new string[] {
                    "HydraPlugin/Public",
				}
				);

			PrivateIncludePaths.AddRange(
				new string[] {
					"HydraPlugin/Private",
				}
				);

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
                    "Engine",
                    "InputCore",
                    "InputDevice",
                    "HeadMountedDisplay",
                    "Projects",
                    "Slate",
                    "SlateCore"
					// ... add other public dependencies that you statically link with here ...
				}
				);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					// ... add private dependencies that you statically link with here ...
				}
				);

			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
					// ... add any modules that your module loads dynamically here ...
				}
				);

            LoadHydraLib(Target);

            //Add DLL for packaging

		}

        public bool LoadHydraLib(TargetInfo Target)
        {
            bool isLibrarySupported = false;

            if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
            {
                isLibrarySupported = true;

                string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x32";
                string LibrariesPath = Path.Combine(ThirdPartyPath, "Sixense", "Lib");

                //Lib based bind unsupported due to sixense wrong lib version compile, limiting platforms to windows 32/64
                //We use a direct dll bind, with data format from .lib
                //PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "sixense_s_" + PlatformString + ".lib"));

                if (Target.Platform == UnrealTargetPlatform.Win64)
                {
                    PublicDelayLoadDLLs.Add("sixense_x64.dll");
                    RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(BinariesPath, "Win64", "sixense_x64.dll")));
                }
                else
                {
                    PublicDelayLoadDLLs.Add("sixense.dll");
                    RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(BinariesPath, "Win32", "sixense.dll")));
                }
            }

            if (isLibrarySupported)
            {
                // Include path
                PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "Sixense", "Include"));
            }


            return isLibrarySupported;
        }
	}

}