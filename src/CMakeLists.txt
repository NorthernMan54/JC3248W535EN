# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/JC3248W535EN/1-Demo/Demo_Arduino/DEMO_LVGL/*.*)

idf_component_register(SRCS ${app_sources})
