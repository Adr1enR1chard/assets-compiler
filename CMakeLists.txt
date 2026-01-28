cmake_minimum_required(VERSION 3.20)

project(AssetCompiler LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

### Fetch Assimp ###
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

# ### Create Loader Library ###
add_library(AssetsLoader INTERFACE)
target_include_directories(AssetsLoader INTERFACE
    ${CMAKE_SOURCE_DIR}/loader/src
)

### Create Compiler Executable ###
add_executable(AssetsCompiler
    compiler/src/main.cpp
    compiler/src/texture_compiler.cpp
    compiler/src/mesh_compiler.cpp
    compiler/src/model_compiler.cpp
    compiler/include/stb_image/stb_image.cpp
)

target_include_directories(AssetsCompiler 
PRIVATE
    ${CMAKE_SOURCE_DIR}/compiler/src
    ${CMAKE_SOURCE_DIR}/compiler/include
)

target_link_libraries(AssetsCompiler PUBLIC
    assimp::assimp
)

enable_testing()

add_test(
    TARGET AssetsCompiler POST_BUILD
    COMMAND AssetsCompiler ${CMAKE_SOURCE_DIR}/test ${CMAKE_BINARY_DIR}/compiled_assets
    COMMENT "Running AssetsCompiler to process assets..."
)