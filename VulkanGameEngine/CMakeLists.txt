cmake_minimum_required(VERSION 3.10)

project(VulkanGameEngine)

set(CMAKE_CXX_STANDARD 17)

# Include Vulkan and GLFW directories if necessary
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

# Add source files
file(GLOB ENGINE_FILES src/engine/*.cpp)
file(GLOB SRC_FILES src/*.cpp)

set(SOURCES ${SRC_FILES} ${ENGINE_FILES})

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/engine)

# Create the executable
add_executable(VGE ${SOURCES})


# Link Vulkan and GLFW libraries
link_directories(/usr/lib/x86_64-linux-gnu)
target_link_libraries(VGE /usr/lib/x86_64-linux-gnu/libglfw.so ${Vulkan_LIBRARIES})
target_include_directories(VGE PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Add precompiled header
target_precompile_headers(VGE PRIVATE ${CMAKE_SOURCE_DIR}/src/pch.h)