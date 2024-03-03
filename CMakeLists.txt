cmake_minimum_required(VERSION 3.22)
project(mika)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Directories
set(MIKA_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(MIKA_EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")

# Add raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
add_subdirectory("${MIKA_EXTERNAL_DIR}/raylib")
# Add fmt
add_subdirectory("${MIKA_EXTERNAL_DIR}/fmt")
# Add LDTkLoader
add_subdirectory("${MIKA_EXTERNAL_DIR}/ldtk")
# Add unordered_dense
add_subdirectory("${MIKA_EXTERNAL_DIR}/hashmap")

# Set C++ standard to 17
set(CMAKE_CXX_STANDARD 17)
# Set C standard to 11
set(CMAKE_C_STANDARD 11)
# Automatically link .rc files for app info on Windows
set(CMAKE_AUTORCC ON)

# Glob all current source files
# TODO: This might be a bad idea, but do it anyway for now
file(GLOB_RECURSE MIKA_SOURCES CONFIGURE_DEPENDS "${MIKA_SOURCE_DIR}/*.cpp" "${MIKA_SOURCE_DIR}/icon.rc")

# Declare executable woohoo
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${MIKA_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${MIKA_SOURCE_DIR})

# Statically link our standard libraries
if(MSVC)
	# Statically link MSVC runtime because fuck everything
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else(MSVC)
	# Self explanatory.
	target_link_libraries(${PROJECT_NAME} PRIVATE -static)
endif(MSVC)

# Link all dependencies
target_link_libraries(mika PRIVATE raylib unordered_dense::unordered_dense fmt::fmt LDtkLoader)

# Set output directory
set_target_properties(mika PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/export"
)