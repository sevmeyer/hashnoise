cmake_minimum_required(VERSION 3.11)
include(FetchContent)

# Target:  minarg
# Include: minarg/minarg.hpp
# Version: 1.0.1
# License: Boost Software License 1.0

# Files
FetchContent_Declare(minarg
	URL "https://github.com/sevmeyer/minarg/raw/v1.0.1/include/minarg/minarg.hpp"
	URL_HASH SHA256=a5c2d2272c2a3db6f4c7083b57f1bc37bbc401ffa449b03145a3db92134b9c42
	DOWNLOAD_DIR "${CMAKE_CURRENT_BINARY_DIR}/minarg"
	DOWNLOAD_NO_EXTRACT TRUE)

# Download
FetchContent_GetProperties(minarg)
if(NOT minarg_POPULATED)
	message(STATUS "Fetching external library: minarg")
	FetchContent_Populate(minarg)
endif()

# Target
add_library(minarg INTERFACE)
target_include_directories(minarg SYSTEM INTERFACE "${CMAKE_CURRENT_BINARY_DIR}")
