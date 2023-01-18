workspace "ModLoader"
    architecture "x86"
    configurations { "Debug", "Release" }

project "ModLoader"
    kind "SharedLib"
    targetextension ".asi"

    language "C++"
    cppdialect "C++17"

    -- Source file
    files "src/**"

    -- Vendor modules
    files { "vendor/minhook/src/**", "vendor/patterns/*.cpp" }
    includedirs { "vendor/minhook/include", "vendor/patterns" }

    symbols "On"

    filter "configurations:Debug"
        defines { "DEBUG", "_DEBUG" }

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
