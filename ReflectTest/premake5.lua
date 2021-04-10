project "ReflectTest"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
	staticruntime "on"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")
    debugdir ("../bin/" .. outputdir .. "/%{prj.name}")


    files
	{
		"src/**.h",
        "src/**.cpp",
	}

    includedirs 
    {
		"$(ProjectDir)src",
        "../Reflect//inc",
	}

    links 
	{ 
        "Reflect",
	}

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines 
        {  
            "PLATFORM_WINDWOS",
        }

    filter "configurations:Debug"
        defines 
        { 
            "DEBUG",
        }
        symbols "on"
        kind "ConsoleApp"


    filter "configurations:Release"
        defines 
        { 
            "RELEASE",
        }
        optimize "on"
        kind "ConsoleApp"

    filter "configurations:Dist"
        defines 
        { 
            "DIST"
        }
        optimize "full"
        kind "ConsoleApp"

        filter { "system:windows", "configurations:Release" }
            buildoptions "/MT"