project "ReflectDLL"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
	staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
    debugdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")

    files
	{
		"../Reflect/inc/**.h",
        "../Reflect/src/**.cpp",
	}

    includedirs 
    {
        "%{IncludeDir.refect}/inc",
	}

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines 
        {  
            "PLATFORM_WINDWOS",
            "REFLECT_DLL_EXPORT",
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
            "DIST"
        }
        optimize "full"

        filter { "system:windows", "configurations:Debug" }
            buildoptions "/MTd"
        filter { "system:windows", "configurations:Release" }
            buildoptions "/MT"
        filter { "system:windows", "configurations:Dist" }
            buildoptions "/MT"