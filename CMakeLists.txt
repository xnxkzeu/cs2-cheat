cmake_minimum_required(VERSION 3.25)

project(cs2-cheat C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(cs2-cheat SHARED
    src/core/helpers/memory.cpp src/core/helpers/memory.hpp
    src/core/helpers/logger.cpp src/core/helpers/logger.hpp
    src/core/helpers/schema.cpp src/core/helpers/schema.hpp
    src/core/helpers/input.cpp src/core/helpers/input.hpp
    src/core/helpers/render.cpp src/core/helpers/render.hpp
    src/core/helpers/entities.cpp src/core/helpers/entities.hpp

    src/core/types/color_t.hpp
    src/core/types/vector_t.hpp
    src/core/types/matrix_t.hpp
    src/core/types/quaternion_t.hpp

    src/core/hash/fnv1a.hpp

    src/core/source2_engine/datatypes/ctransform.hpp
    src/core/source2_engine/datatypes/cutlmemory.hpp
    src/core/source2_engine/datatypes/cutlvector.hpp
    src/core/source2_engine/datatypes/cutlstring.hpp
    src/core/source2_engine/datatypes/cutlmemorypool.hpp
    src/core/source2_engine/datatypes/cutltshash.hpp
    src/core/source2_engine/datatypes/cinterlockedint.hpp
    src/core/source2_engine/datatypes/resourcebinding_t.hpp
    src/core/source2_engine/datatypes/cstronghandle.hpp
    src/core/source2_engine/datatypes/centityhandle.hpp
    src/core/source2_engine/datatypes/cgamescenenode.hpp
    src/core/source2_engine/datatypes/cskeletoninstance.hpp
    src/core/source2_engine/datatypes/ccollisionproperty.hpp
    src/core/source2_engine/datatypes/cmodel.hpp

    src/core/source2_engine/entities/centityinstance.hpp
    src/core/source2_engine/entities/c_baseentity.hpp
    src/core/source2_engine/entities/c_basemodelentity.hpp
    src/core/source2_engine/entities/cbaseanimgraph.hpp
    src/core/source2_engine/entities/c_baseflex.hpp
    src/core/source2_engine/entities/c_basecombatcharacter.hpp
    src/core/source2_engine/entities/c_baseplayerpawn.hpp
    src/core/source2_engine/entities/c_csplayerpawn.hpp
    src/core/source2_engine/entities/cbaseplayercontroller.hpp
    src/core/source2_engine/entities/ccsplayercontroller.hpp

    src/core/source2_engine/iappsystem.hpp
    src/core/source2_engine/isource2engine.hpp
    src/core/source2_engine/isource2client.hpp
    src/core/source2_engine/iinputsystem.hpp
    src/core/source2_engine/ischemasystem.hpp
    src/core/source2_engine/crenderdevicedx11.hpp
    src/core/source2_engine/cgameentitysystem.hpp
    src/core/source2_engine/crendergamesystem.hpp

    src/core/hooks/functions/framestagenotify.cpp
    src/core/hooks/functions/mouseinputenabled.cpp
    src/core/hooks/functions/present.cpp
    src/core/hooks/functions/resizebuffers.cpp

    src/core/ui/menu.cpp src/core/ui/menu.hpp

    src/core/hooks/hooks.cpp src/core/hooks/hooks.hpp

    src/context/cs2.hpp
    src/context/memory.hpp
    src/context/config.hpp

    src/features/esp.cpp src/features/esp.hpp

    src/main.cpp)

find_package(spdlog CONFIG REQUIRED)
find_package(minhook CONFIG REQUIRED)
find_package(Freetype CONFIG REQUIRED)
find_package(unofficial-lunasvg CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_path(XORSTR_INCLUDE_DIRS "xorstr.hpp")

target_link_libraries(cs2-cheat PRIVATE
    spdlog::spdlog
    minhook::minhook
    Freetype::Freetype
    unofficial::lunasvg::lunasvg
    imgui::imgui)

target_include_directories(cs2-cheat PRIVATE
    ${XORSTR_INCLUDE_DIRS})

install(TARGETS cs2-cheat)
install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION bin OPTIONAL)

target_compile_definitions(cs2-cheat PRIVATE
    PROJECT_NAME="${CMAKE_PROJECT_NAME}"

    _ITERATOR_DEBUG_LEVEL=0

    NOMINMAX
    WIN32_LEAN_AND_MEAN
    VC_EXTRALEAN
    _CRT_SECURE_NO_WARNINGS
    UNICODE
    _UNICODE

    JM_XORSTR_DISABLE_AVX_INTRINSICS)

target_link_options(cs2-cheat PRIVATE LINKER:/ENTRY:DllEntryPoint)

target_compile_options(cs2-cheat PRIVATE
    -Wall
    -Wpedantic
    -Wshadow
    -Wextra)