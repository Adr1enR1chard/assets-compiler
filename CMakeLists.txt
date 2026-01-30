cmake_minimum_required(VERSION 3.20)

project(AssetCompiler LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
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
# Formats
add_library(assets_format INTERFACE)
target_include_directories(assets_format INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/formats/include
)
target_link_libraries(assets_format INTERFACE
    thirdparty_glm
)

# Assets Loader (header-only)
add_library(assets_loader 
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/assets_loader/src/texture_loader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/assets_loader/src/model_loader.cpp
)
target_include_directories(assets_loader PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/assets_loader/include
)
target_link_libraries(assets_loader PUBLIC assets_format)

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
add_executable(assets_compiler
    tools/assets_compiler/src/main.cpp
    tools/assets_compiler/src/texture_compiler.cpp
    tools/assets_compiler/src/model_compiler.cpp
)

target_link_libraries(assets_compiler
    PRIVATE
        assets_format
        thirdparty_glm
        thirdparty_stb
        assimp::assimp
)

target_include_directories(assets_compiler
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/assets_compiler/include
)

if (PROJECT_IS_TOP_LEVEL)
    add_executable(
        assets_loader_test
        test/main.cpp
    )
    target_link_libraries(
        assets_loader_test
        PRIVATE
            assets_loader
            thirdparty_glm
            thirdparty_stb
    )
    set_target_properties(assets_loader_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests_bin
    )
    
    enable_testing()

    add_test(
        NAME assets_compiler_test
        COMMAND assets_compiler ${CMAKE_CURRENT_SOURCE_DIR}/test "$<TARGET_FILE_DIR:assets_loader_test>/assets"
    )

    add_test(
        NAME assets_loader_test
        COMMAND assets_loader_test
        WORKING_DIRECTORY $<TARGET_FILE_DIR:assets_loader_test>
    )
endif()