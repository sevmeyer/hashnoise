cmake_minimum_required(VERSION 3.11)
include(FetchContent)

# Target:  stb_image_write
# Include: stb_image_write/stb_image_write.h
# Version: 1.13
# License: Public Domain

# Files
FetchContent_Declare(stb_image_write
	URL "https://github.com/nothings/stb/raw/2c2908f50515dcd939f24be261c3ccbcd277bb49/stb_image_write.h"
	URL_HASH SHA256=0e8b3d80bc6eb8fdb64abc4db9fec608b489bc73418eaf14beda102a0699a4c9
	DOWNLOAD_DIR "${CMAKE_CURRENT_BINARY_DIR}/stb_image_write"
	DOWNLOAD_NO_EXTRACT TRUE)

# Download
FetchContent_GetProperties(stb_image_write)
if(NOT stb_image_write_POPULATED)
	message(STATUS "Fetching external library: stb_image_write")
	FetchContent_Populate(stb_image_write)
endif()

# Target
add_library(stb_image_write INTERFACE)
target_include_directories(stb_image_write SYSTEM INTERFACE "${CMAKE_CURRENT_BINARY_DIR}")

# The header documentation recommends to
# disable strict aliasing optimizations
if(NOT MSVC)
	target_compile_options(stb_image_write INTERFACE -fno-strict-aliasing)
endif()
