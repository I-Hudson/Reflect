project "Reflect"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
	staticruntime "on"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")
    debugdir ("../bin/" .. outputdir .. "/%{prj.name}")

    files
	{
        "src/**.cpp",
        "inc/**.h"
	}

    includedirs 
    {
		"$(ProjectDir)inc",
	}

    filter "system:windows"
        systemversion "latest"

        defines 
        {  
            "PlatformWindows",
        }

    filter "configurations:Debug"
        defines
        {  
            "DEBUG",
        }
        symbols "on"

    filter "configurations:Release"
        defines 
        {
            "RELEASE",
        }
        optimize "on"

    filter "configurations:Dist"
        defines 
        { 
            "DIST",
        }
        optimize "full"