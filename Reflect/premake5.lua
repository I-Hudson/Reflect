project "Reflect"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
	staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
    debugdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")

    files
	{
        "src/**.cpp",
        "inc/**.h"
	}

    includedirs 
    {
		"$(ProjectDir)inc",
        "%{IncludeDir.dirent}",
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

    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"