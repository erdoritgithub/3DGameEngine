workspace "Hazel"
	architecture "x64"
	targetdir "build"
	
	configurations 
	{ 
		"Debug", 
        "Release",
        "Dist"
    }

	flags
	{
		"MultiProcessorCompile"
	}

	startproject "Hazelnut"
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"
IncludeDir["Glad"] = "Hazel/vendor/Glad/include"
IncludeDir["ImGui"] = "Hazel/vendor/ImGui"
IncludeDir["glm"] = "Hazel/vendor/glm"
IncludeDir["Box2D"] = "Hazel/vendor/Box2D/include"
IncludeDir["entt"] = "Hazel/vendor/entt/include"
IncludeDir["FastNoise"] = "Hazel/vendor/FastNoise"
IncludeDir["mono"] = "Hazel/vendor/mono/include"
IncludeDir["PhysX"] = "Hazel/vendor/PhysX/include"

LibraryDir = {}
LibraryDir["mono"] = "vendor/mono/lib/mono-2.0-sgen.lib"
LibraryDir["PhysX_LowLevel"] = "vendor/PhysX/lib/%{cfg.buildcfg}/LowLevel.lib"
LibraryDir["PhysX_LowLevelAABB"] = "vendor/PhysX/lib/%{cfg.buildcfg}/LowLevelAABB.lib"
LibraryDir["PhysX_LowLevelDynamics"] = "vendor/PhysX/lib/%{cfg.buildcfg}/LowLevelDynamics.lib"
LibraryDir["PhysX"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysX_static_64.lib"
LibraryDir["PhysXCharacterKinematic"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysXCharacterKinematic_static_64.lib"
LibraryDir["PhysXCommon"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysXCommon_static_64.lib"
LibraryDir["PhysXCooking"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysXCooking_static_64.lib"
LibraryDir["PhysXExtensions"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysXExtensions_static_64.lib"
LibraryDir["PhysXFoundation"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysXFoundation_static_64.lib"
LibraryDir["PhysXPvd"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysXPvdSDK_static_64.lib"
LibraryDir["PhysXTask"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysXTask.lib"
LibraryDir["PhysXVehicle"] = "vendor/PhysX/lib/%{cfg.buildcfg}/PhysXVehicle_static_64.lib"
LibraryDir["PhysX_SceneQuery"] = "vendor/PhysX/lib/%{cfg.buildcfg}/SceneQuery.lib"
LibraryDir["PhysX_SimulationController"] = "vendor/PhysX/lib/%{cfg.buildcfg}/SimulationController.lib"


group "Dependencies"
include "Hazel/vendor/GLFW"
include "Hazel/vendor/Glad"
include "Hazel/vendor/ImGui"
include "Hazel/vendor/Box2D"
group ""


group "Core"
project "Hazel"
    location "Hazel"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    buildoptions "/utf-8"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "hzpch.h"
    pchsource "Hazel/src/hzpch.cpp"

	files 
	{ 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.c", 
		"%{prj.name}/src/**.hpp", 
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/FastNoise/**.cpp",
		"%{prj.name}/vendor/yaml-cpp/src/**.cpp",
		"%{prj.name}/vendor/yaml-cpp/src/**.h",
		"%{prj.name}/vendor/yaml-cpp/include/yaml-cpp/**.h"
    }

    includedirs
	{
		"%{prj.name}/src",
        "%{prj.name}/vendor",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
		"%{IncludeDir.Box2D}",
        "%{IncludeDir.entt}",
		"%{IncludeDir.mono}",
		"%{IncludeDir.FastNoise}",
		"%{IncludeDir.PhysX}",
        "%{prj.name}/vendor/assimp/include",
        "%{prj.name}/vendor/stb/include",
		"%{prj.name}/vendor/yaml-cpp/include"
    }
    
    links 
	{ 
        "GLFW",
        "Glad",
        "ImGui",
		"Box2D",
        "opengl32.lib",
		"%{LibraryDir.mono}",
		"%{LibraryDir.PhysX_LowLevel}",
		"%{LibraryDir.PhysX_LowLevelAABB}",
		"%{LibraryDir.PhysX_LowLevelDynamics}",
		"%{LibraryDir.PhysX}",
		"%{LibraryDir.PhysXCharacterKinematic}",
		"%{LibraryDir.PhysXCommon}",
		"%{LibraryDir.PhysXCooking}",
		"%{LibraryDir.PhysXExtensions}",
		"%{LibraryDir.PhysXFoundation}",
		"%{LibraryDir.PhysXPvd}",
		"%{LibraryDir.PhysXTask}",
		"%{LibraryDir.PhysXVehicle}",
		"%{LibraryDir.PhysX_SceneQuery}",
		"%{LibraryDir.PhysX_SimulationController}"
    }

	defines
	{
		"PX_PHYSX_STATIC_LIB"
	}

    filter "files:Hazel/vendor/FastNoise/**.cpp or files:Hazel/vendor/yaml-cpp/src/**.cpp"
        flags { "NoPCH" }
    
	filter "system:windows"
        systemversion "latest"
        
		defines 
		{ 
            "HZ_PLATFORM_WINDOWS",
            "HZ_BUILD_DLL"
		}

    filter "configurations:Debug"
        defines "HZ_DEBUG"
        symbols "On"
                
    filter "configurations:Release"
		defines
		{
			"HZ_RELEASE",
			"NDEBUG" -- PhysX Requires This
		}
        optimize "On"

    filter "configurations:Dist"
        defines "HZ_DIST"
        optimize "On"


project "Hazel-ScriptCore"
	location "Hazel-ScriptCore"
	kind "SharedLib"
	language "C#"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	files 
	{
		"%{prj.name}/src/**.cs", 
	}
group ""


group "Tools"
project "Hazelnut"
    location "Hazelnut"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    buildoptions "/utf-8"
    
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	links 
	{ 
        "Hazel"
    }
    
	files 
	{ 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.c", 
		"%{prj.name}/src/**.hpp", 
		"%{prj.name}/src/**.cpp" 
	}
    
	includedirs 
	{
        "%{prj.name}/src",
        "Hazel/src",
        "Hazel/vendor",
        "%{IncludeDir.entt}",
        "%{IncludeDir.glm}"
    }

    postbuildcommands 
	{
		'{COPY} "../Hazelnut/assets" "%{cfg.targetdir}/assets"'
	}


	filter "system:windows"
        systemversion "latest"
                
		defines 
		{ 
            "HZ_PLATFORM_WINDOWS"
		}
    
    filter "configurations:Debug"
        defines "HZ_DEBUG"
        symbols "on"

		links
		{
			"Hazel/vendor/assimp/lib/Debug/assimp.lib"
		}

        postbuildcommands 
        {
            '{COPY} "../Hazel/vendor/assimp/bin/Debug/assimp-vc143-mtd.dll" "%{cfg.targetdir}"',
            '{COPY} "../Hazel/vendor/mono/bin/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
        }
                
    filter "configurations:Release"
        defines "HZ_RELEASE"
        optimize "on"

		links
		{
			"Hazel/vendor/assimp/lib/Release/assimp.lib"
		}

        postbuildcommands 
        {
            '{COPY} "../Hazel/vendor/assimp/bin/Debug/assimp-vc143-mtd.dll" "%{cfg.targetdir}"',
            '{COPY} "../Hazel/vendor/mono/bin/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
        }

    filter "configurations:Dist"
        defines "HZ_DIST"
        optimize "on"

		links
		{
			"Hazel/vendor/assimp/lib/Release/assimp.lib"
        }

        postbuildcommands 
        {
            '{COPY} "../Hazel/vendor/assimp/bin/Debug/assimp-vc143-mtd.dll" "%{cfg.targetdir}"',
            '{COPY} "../Hazel/vendor/mono/bin/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
        }
        
group ""


workspace "Sandbox"
	architecture "x64"
	targetdir "build"
	
	configurations 
	{ 
		"Debug", 
		"Release",
		"Dist"
	}


project "Hazel-ScriptCore"
	location "Hazel-ScriptCore"
	kind "SharedLib"
	language "C#"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	files 
	{
		"%{prj.name}/src/**.cs", 
	}


project "ExampleApp"
	location "ExampleApp"
	kind "SharedLib"
	language "C#"
	targetdir ("Hazelnut/assets/scripts")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	files 
	{
		"%{prj.name}/src/**.cs", 
	}
	links
	{
		"Hazel-ScriptCore"
	}

-- project "Sandbox"
--     location "Sandbox"
--     kind "ConsoleApp"
--     language "C++"
--     cppdialect "C++17"
--     staticruntime "on"
--     buildoptions "/utf-8"
    
--     targetdir ("bin/" .. outputdir .. "/%{prj.name}")
--     objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

--     links 
--     { 
--         "Hazel"
--     }
    
--     files 
--     { 
--         "%{prj.name}/src/**.h", 
--         "%{prj.name}/src/**.c", 
--         "%{prj.name}/src/**.hpp", 
--         "%{prj.name}/src/**.cpp" 
--     }
    
--     includedirs 
--     {
--         "%{prj.name}/src",
--         "Hazel/src",
--         "Hazel/vendor",
--         "%{IncludeDir.glm}"
--     }
    
--     filter "system:windows"
--         systemversion "latest"
                
--         defines 
--         { 
--             "HZ_PLATFORM_WINDOWS"
--         }
    
--     filter "configurations:Debug"
--         defines "HZ_DEBUG"
--         symbols "on"

--         links
--         {
--             "Hazel/vendor/assimp/lib/Debug/assimp.lib"
--         }
                
--     filter "configurations:Release"
--         defines "HZ_RELEASE"
--         optimize "on"

--         links
--         {
--             "Hazel/vendor/assimp/lib/Release/assimp.lib"
--         }

--     filter "configurations:Dist"
--         defines "HZ_DIST"
--         optimize "on"

--         links
--         {
--             "Hazel/vendor/assimp/lib/Release/assimp.lib"
--         }

