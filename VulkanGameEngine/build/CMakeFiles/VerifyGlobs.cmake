# CMAKE generated file: DO NOT EDIT!
# Generated by CMake Version 3.28
cmake_policy(SET CMP0009 NEW)

# SRC_FILES at CMakeLists.txt:46 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/*.c")
set(OLD_GLOB
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/build/CMakeFiles/cmake.verify_globs")
endif()

# SRC_FILES at CMakeLists.txt:46 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/*.cpp")
set(OLD_GLOB
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_buffer.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_descriptors.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_device.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_engine.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_pipeline.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_renderer.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_swap_chain.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_window.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/VGE_app.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/VGE_frame_info.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/VGE_mesh.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/components/component.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/components/component_manager.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/game_object.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/gui/gui_manager.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/render_systems/VGE_default_render_system.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/render_systems/VGE_pointlight_render_system.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/scene/scene.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/scene/scene_manager.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/scene/scenes/default_scene.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/scene/scenes/default_scene_2.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/main.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/pch.cpp"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/build/CMakeFiles/cmake.verify_globs")
endif()

# SRC_FILES at CMakeLists.txt:46 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/*.h")
set(OLD_GLOB
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/pch.h"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/build/CMakeFiles/cmake.verify_globs")
endif()

# SRC_SUBDIRS at CMakeLists.txt:63 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES true "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/*")
set(OLD_GLOB
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_buffer.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_buffer.hpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_descriptors.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_descriptors.hpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_device.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_device.hpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_engine.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_engine.hpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_pipeline.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_pipeline.hpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_renderer.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_renderer.hpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_swap_chain.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_swap_chain.hpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_window.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/engine/VGE_window.hpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/VGE_app.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/VGE_app.hpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/VGE_frame_info.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/VGE_frame_info.hpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/VGE_mesh.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/VGE_mesh.hpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/components"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/components/component.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/components/component.hpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/components/component_manager.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/components/component_manager.hpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/game_object.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/game_object.hpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/gui"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/gui/gui_manager.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/gui/gui_manager.hpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/render_systems"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/render_systems/VGE_default_render_system.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/render_systems/VGE_default_render_system.hpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/render_systems/VGE_pointlight_render_system.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/render_systems/VGE_pointlight_render_system.hpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/scene"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/scene/scene.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/scene/scene.hpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/scene/scene_manager.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/scene/scene_manager.hpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/scene/scenes"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/scene/scenes/default_scene.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/scene/scenes/default_scene.hpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/scene/scenes/default_scene_2.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/game/scene/scenes/default_scene_2.hpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/main.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/pch.cpp"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/pch.h"
  "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/src/pch.pch"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/rcwsl/VulkanGameEngine/Game_Engine_II/VulkanGameEngine/build/CMakeFiles/cmake.verify_globs")
endif()
