cmake_minimum_required(VERSION 3.11)

option(SEEDSEARCH "Compile seedsearch executable" OFF)
if(SEEDSEARCH)

	# Add <thread> support
	set(THREADS_PREFER_PTHREAD_FLAG TRUE)
	find_package(Threads REQUIRED)

	# Add zlib
	find_package(ZLIB REQUIRED)

	# Executable
	add_executable(seedsearch "main.cpp")
	target_link_libraries(seedsearch PRIVATE hashnoise minarg Threads::Threads ${ZLIB_LIBRARIES})
	target_include_directories(seedsearch PRIVATE ${ZLIB_INCLUDE_DIRS})

endif()
