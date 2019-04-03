cmake_minimum_required(VERSION 3.11)

project(hashnoise)

# Library target
add_library(hashnoise INTERFACE)
target_include_directories(hashnoise INTERFACE "include")
target_compile_features(hashnoise INTERFACE cxx_std_11)

# Include tests in top-level build
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	add_subdirectory("extern/minarg")
	add_subdirectory("extern/stb_image_write")
	add_subdirectory("test")
	add_subdirectory("seedsearch")
endif ()
