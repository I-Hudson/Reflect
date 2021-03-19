project "ReflectEXE"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
	staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
    debugdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")


    files
	{
        "src/*.cpp",
		"../Reflect/src/**.h",
        "../Reflect/src/**.cpp",
	}

    includedirs 
    {
        "%{IncludeDir.refect}/src",
	}

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines 
        {  
            "PLATFORM_WINDWOS",
            "REFLECT_EXE",
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