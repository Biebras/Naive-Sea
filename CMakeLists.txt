# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.10)

# Project's name
project(NaiveSea)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 14)

# Include header files located in the 'include' directory
include_directories(include)
# Include imgui headers
include_directories(vendor/include/imgui)
# Include imGuizmo headers
include_directories(vendor/include/imGuizmo)

# Include header files located in the 'vendor/include' directory
include_directories(vendor/include)

# Link the 'lib' directory
link_directories(vendor/lib)

# Add an executables to be built from the source files listed in the command invocation.
file(GLOB_RECURSE SOURCES "src/*.cpp" "vendor/src/**/*.c" "vendor/src/**/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

# Locate the GLFW library on your system
find_package(glfw3 3.3 REQUIRED)
# Locate the Assimp library on your system
find_library(ASSIMP_LIBRARY NAMES assimp libassimp PATHS "${PROJECT_SOURCE_DIR}/vendor/lib/assimp")

# Link your application with the GLFW library
target_link_libraries(${PROJECT_NAME} glfw)
# Link your application with the Assimp library
target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIBRARY})
