cmake_minimum_required(VERSION 3.10)

add_library(glbasimac STATIC)
add_library(tools STATIC)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE GLBASIMAC_SOURCES CONFIGURE_DEPENDS src/*)
file(GLOB_RECURSE TOOLS_SOURCES CONFIGURE_DEPENDS src/*)
if (MSVC)
target_compile_options(glbasimac PRIVATE /W3)
target_compile_options(tools PRIVATE /W3)
else()
target_compile_options(glbasimac PRIVATE -std=c++17 -Wall -Wextra)
target_compile_options(tools PRIVATE -std=c++17 -Wall -Wextra)
endif()

target_sources(glbasimac PRIVATE ${GLBASIMAC_SOURCES})
target_sources(tools PRIVATE ${GLBASIMAC_SOURCES})
target_include_directories(glbasimac PUBLIC ../glbasimac/)
target_include_directories(tools PUBLIC ../tools/)
include_directories(glbasimac)
include_directories(tools)

