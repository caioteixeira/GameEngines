// Some copyright should be here...

using UnrealBuildTool;

public class ITPMeshExporter : ModuleRules
{
	public ITPMeshExporter(TargetInfo Target)
	{
		
		PublicIncludePaths.AddRange(
			new string[] {
				"ITPMeshExporter/Public"
				
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"ITPMeshExporter/Private",
				
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
                "Engine",
                "RenderCore",
                "UnrealEd",
                "ImageWrapper",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Slate", "SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
