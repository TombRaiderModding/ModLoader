workspace "ModLoader"
    architecture "x86"
    configurations { "Debug", "Release" }

project "ModLoader"
    kind "SharedLib"
    targetextension ".asi"

    language "C++"
    cppdialect "C++17"

    -- Source files
    files "src/**"
    includedirs { "src" }

    -- Vendor files
    files { "vendor/minhook/src/**", "vendor/patterns/*.cpp" }
    includedirs { "vendor/minhook/include", "vendor/patterns" }

    symbols "On"

    -- Windows XP support
    buildoptions { "/Zc:threadSafeInit-" }

    filter "configurations:Debug"
        defines { "DEBUG", "_DEBUG" }

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
