cmake_minimum_required(VERSION 3.10)

project(VulkanGameEngine)

set(CMAKE_CXX_STANDARD 17)

# Include Vulkan and GLFW directories if necessary
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

# Add source files
file(GLOB SOURCES src/*.cpp)

# Create the executable
add_executable(VulkanGameEngine ${SOURCES})

# Link Vulkan and GLFW libraries
link_directories(/usr/lib/x86_64-linux-gnu)
target_link_libraries(VulkanGameEngine /usr/lib/x86_64-linux-gnu/libglfw.so ${Vulkan_LIBRARIES})
