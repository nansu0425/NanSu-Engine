workspace "NanSu-Engine"
    architecture "x64"
    startproject "Editor"

    configurations { "Debug", "Release", "Distribution" }

    -- 결과물 출력 경로 (예: Binaries/Debug-windows-x64/Editor/)
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    -- 공통 설정
    filter "system:windows"
        systemversion "latest"
        defines { "NS_PLATFORM_WINDOWS" }

    filter "configurations:Debug"
        defines { "NS_DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NS_RELEASE" }
        runtime "Release"
        optimize "On"

    filter "configurations:Distribution"
        defines { "NS_DISTRIBUTION" }
        runtime "Release"
        optimize "On"

--------------------------------------------------------------------------------
-- 1. Engine (라이브러리)
--------------------------------------------------------------------------------
project "Engine"
    location "Source/Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "On"

    targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("Build/" .. outputdir .. "/%{prj.name}")

    files {
        "Source/Engine/**.h",
        "Source/Engine/**.cpp"
    }

    includedirs {
        "Source",
        "Source/Engine",
        "ThirdParty/spdlog/include"
    }

    buildoptions { "/utf-8" }

--------------------------------------------------------------------------------
-- 2. Editor (실행 파일)
--------------------------------------------------------------------------------
project "Editor"
    location "Source/Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "On"

    targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("Build/" .. outputdir .. "/%{prj.name}")

    files {
        "Source/Editor/**.h",
        "Source/Editor/**.cpp"
    }

    includedirs {
        "Source",
        "Source/Engine",
        "ThirdParty/spdlog/include"
    }

    links { "Engine" }

    buildoptions { "/utf-8" }

--------------------------------------------------------------------------------
-- 3. Game (샌드박스)
--------------------------------------------------------------------------------
project "Game"
    location "Source/Game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "On"

    targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("Build/" .. outputdir .. "/%{prj.name}")

    files {
        "Source/Game/**.h",
        "Source/Game/**.cpp"
    }

    includedirs {
        "Source",
        "Source/Engine",
        "ThirdParty/spdlog/include"
    }

    links { "Engine" }

    buildoptions { "/utf-8" }