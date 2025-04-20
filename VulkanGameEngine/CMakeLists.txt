cmake_minimum_required(VERSION 3.10)

project(VulkanGameEngine)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_TARGET_NAME VGE)

# Function to copy full resource folders (e.g., assets, textures)
function(copy_resources ${PROJECT_TARGET_NAME})
    foreach(FOLDER IN LISTS ARGN)
        add_custom_command(
            TARGET ${PROJECT_TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/${FOLDER}
                    $<TARGET_FILE_DIR:${PROJECT_TARGET_NAME}>/${FOLDER}
            COMMENT "Copying folder: ${FOLDER} to binary output directory"
        )
    endforeach()
endfunction()

# Function to copy only .spv files from a shader directory
function(copy_spv_shaders ${PROJECT_TARGET_NAME} SHADER_DIR)
    file(GLOB SPV_FILES
        "${CMAKE_SOURCE_DIR}/${SHADER_DIR}/*.spv"
    )

    foreach(FILE_PATH IN LISTS SPV_FILES)
        get_filename_component(FILE_NAME ${FILE_PATH} NAME)

        add_custom_command(
            TARGET ${PROJECT_TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                    "${FILE_PATH}"
                    "$<TARGET_FILE_DIR:${PROJECT_TARGET_NAME}>/${SHADER_DIR}/${FILE_NAME}"
            COMMENT "Copying SPIR-V shader: ${FILE_NAME}"
        )
    endforeach()
endfunction()

# Include Vulkan and GLFW directories if necessary
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

# Add source files
file(GLOB ENGINE_FILES src/engine/*.cpp)
file(GLOB GAME_FILES src/game/*.cpp)
file(GLOB SRC_FILES src/*.cpp)

set(SOURCES ${SRC_FILES} ${ENGINE_FILES} ${GAME_FILES})

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/engine)
include_directories(${CMAKE_SOURCE_DIR}/src/game)
include_directories(${CMAKE_SOURCE_DIR}/assets)

# Create the executable
add_executable(${PROJECT_TARGET_NAME} ${SOURCES})

# Copy resource folders to binary output dir
copy_resources(${PROJECT_TARGET_NAME} assets)
# Copy SPIR-V shaders to binary output dir
copy_spv_shaders(${PROJECT_TARGET_NAME} shaders)

# Link Vulkan and GLFW libraries
link_directories(/usr/lib/x86_64-linux-gnu)
target_link_libraries(${PROJECT_TARGET_NAME} /usr/lib/x86_64-linux-gnu/libglfw.so ${Vulkan_LIBRARIES})
target_include_directories(${PROJECT_TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Add precompiled header
target_precompile_headers(${PROJECT_TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/pch.h)