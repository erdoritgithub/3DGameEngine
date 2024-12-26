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
project "Hazel"
    location "Hazel"
    kind "StaticLib"
    language "C++"
    buildoptions "/utf-8"
    
	targetdir ("bin/" .. outputdir .. "/Hazel")
    objdir ("bin-int/" .. outputdir .. "/Hazel")
	files 
	{ 
		"Hazel/**.h", 
		"Hazel/**.c", 
		"Hazel/**.hpp", 
		"Hazel/**.cpp" 
    }

    includedirs
	{
		"Hazel/src",
		"Hazel/vendor",
	}
    
	filter "system:windows"
		cppdialect "C++17"
        staticruntime "On"
        
		defines 
		{ 
            "HZ_PLATFORM_WINDOWS",
            "HAZEL_BUILD_DLL",
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
    buildoptions "/utf-8"
    
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	dependson 
	{ 
		"Hazel"
    }
    
	files 
	{ 
		"%{prj.name}/**.h", 
		"%{prj.name}/**.c", 
		"%{prj.name}/**.hpp", 
		"%{prj.name}/**.cpp" 
	}
    
	includedirs 
	{
        "%{prj.name}/src",
        "Hazel/src",
        "Hazel/vendor",
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
          