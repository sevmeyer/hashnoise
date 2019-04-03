cmake_minimum_required(VERSION 3.11)

# Test executable
add_executable(hashnoise-test "main.cpp")
target_link_libraries(hashnoise-test PRIVATE hashnoise minarg stb_image_write)

# Language properties
set_property(TARGET hashnoise-test PROPERTY CXX_STANDARD_REQUIRED TRUE)
set_property(TARGET hashnoise-test PROPERTY CXX_EXTENSIONS FALSE)

# Verbose compiler warnings
if(MSVC)
	target_compile_options(hashnoise-test PRIVATE /W4 /WX)
else()
	target_compile_options(hashnoise-test PRIVATE -Wall -Wextra -Werror -pedantic)
endif()
