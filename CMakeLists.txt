cmake_minimum_required(VERSION 3.20)

project(AssetCompiler LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)


###################
#### Dependencies 
###################

#### Assimp ####
include(FetchContent)
FetchContent_Declare(assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG master
)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(assimp)

####################
#### Libraries 
####################
add_library(assets_loader INTERFACE)
target_include_directories(assets_loader INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# GLM (header-only)
add_library(thirdparty_glm INTERFACE)

target_include_directories(thirdparty_glm
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

# stb_image
add_library(thirdparty_stb 
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/stb_image/stb_image.cpp
)

target_include_directories(thirdparty_stb
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

####################
#### Executables
####################
add_executable(asset_compiler
    tools/assets_compiler/src/main.cpp
    tools/assets_compiler/src/mesh_compiler.cpp
    tools/assets_compiler/src/texture_compiler.cpp
    tools/assets_compiler/src/model_compiler.cpp
)

target_link_libraries(asset_compiler
    PRIVATE
        thirdparty_glm
        thirdparty_stb
        assimp::assimp
)

target_include_directories(asset_compiler
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/assets_compiler/include
)


if (PROJECT_IS_TOP_LEVEL)
    enable_testing()

    add_test(
        NAME assets_compiler_test
        COMMAND asset_compiler ${CMAKE_CURRENT_SOURCE_DIR}/test ${CMAKE_SOURCE_BINARY_DIR}/compiled_assets
    )
endif()