workspace "Hazel"
	architecture "x64"
	targetdir "build"
	
	configurations 
	{ 
		"Debug", 
        "Release",
        "Dist"
    }
    
local outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"
IncludeDir["Glad"] = "Hazel/vendor/Glad/include"
IncludeDir["ImGui"] = "Hazel/vendor/ImGui"
IncludeDir["glm"] = "Hazel/vendor/glm"

include "Hazel/vendor/GLFW"
include "Hazel/vendor/Glad"
include "Hazel/vendor/ImGui"

project "Hazel"
    location "Hazel"
    kind "StaticLib"
    language "C++"
    systemversion "latest"
    buildoptions "/utf-8"
    
	targetdir ("bin/" .. outputdir .. "/Hazel")
    objdir ("bin-int/" .. outputdir .. "/Hazel")

    pchheader "hzpch.h"
    pchsource "Hazel/src/hzpch.cpp"

	files 
	{ 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.c", 
		"%{prj.name}/src/**.hpp", 
		"%{prj.name}/src/**.cpp" 
    }

    includedirs
	{
		"Hazel/src",
		"Hazel/vendor",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}"
	}

    links 
	{ 
		"GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }
    
	filter "system:windows"
		cppdialect "C++17"
        staticruntime "On"
        
		defines 
		{ 
            "HZ_PLATFORM_WINDOWS"
		}
					
    filter "configurations:Debug"
        defines "HZ_DEBUG"
        symbols "On"
                
    filter "configurations:Release"
        defines "HZ_RELEASE"
        optimize "On"
    filter "configurations:Dist"
        defines "HZ_DIST"
        optimize "On"
    
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    systemversion "latest"
    buildoptions "/utf-8"
    
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
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
        "%{IncludeDir.glm}"
    }
	
	filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        
		links 
		{ 
			"Hazel"
		}
        
		defines 
		{ 
            "HZ_PLATFORM_WINDOWS",
		}
    
    filter "configurations:Debug"
        defines "HZ_DEBUG"
        symbols "On"
                
    filter "configurations:Release"
        defines "HZ_RELEASE"
        optimize "On"
    filter "configurations:Dist"
        defines "HZ_DIST"
        optimize "On"
          