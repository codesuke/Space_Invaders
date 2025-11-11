cmake_minimum_required(VERSION 3.10)
project(SpaceInvaders)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add the main executable
add_executable(space_invaders space_invaders/main.cpp)

# Setup GLEW
set(GLEW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/utils/glew-2.1.0/include)
set(GLEW_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/utils/glew-2.1.0/lib/Release)

# Determine the correct GLEW library based on compiler
if(MSVC)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(GLEW_LIBRARY ${GLEW_LIBRARY_DIR}/x64/glew32.lib)
    else()
        set(GLEW_LIBRARY ${GLEW_LIBRARY_DIR}/Win32/glew32.lib)
    endif()
elseif(MINGW OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(GLEW_LIBRARY ${GLEW_LIBRARY_DIR}/libglew32.a)
else()
    set(GLEW_LIBRARY ${GLEW_LIBRARY_DIR}/libGLEW.a)
endif()

target_compile_definitions(space_invaders PRIVATE GLEW_STATIC)
target_include_directories(space_invaders PRIVATE ${GLEW_INCLUDE_DIR})
target_link_libraries(space_invaders PRIVATE ${GLEW_LIBRARY})

# Setup GLFW using CMake subdirectory
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
add_subdirectory(utils/glfw-3.4)

target_link_libraries(space_invaders PRIVATE glfw)

# Setup GLM (header-only)
set(GLM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/utils/glm-1.0.2)
target_include_directories(space_invaders PRIVATE ${GLM_INCLUDE_DIR})

# Link OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(space_invaders PRIVATE OpenGL::GL)

# Platform-specific settings
if(MSVC)
    # MSVC-specific configurations
    target_compile_options(space_invaders PRIVATE /W4 /D_CRT_SECURE_NO_WARNINGS)
else()
    # GCC/Clang configurations (including MinGW)
    target_compile_options(space_invaders PRIVATE -Wall -Wextra)
endif()

# Print build information
message(STATUS "Building Space Invaders")
message(STATUS "GLEW: ${GLEW_LIBRARY}")
message(STATUS "GLFW: Using subdirectory build")
message(STATUS "GLM: Header-only library")
