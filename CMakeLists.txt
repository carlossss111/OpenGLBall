cmake_minimum_required (VERSION 3.22)

##############################
#      PROJECT SETTINGS      #
##############################
# Main Settings
set(PROJECT_NAME "Graphics")
set(TARGET_GPU "NVIDIA")
set(DEFAULT_WINDOW_WIDTH 640)
set(DEFAULT_WINDOW_HEIGHT 480)
set(MSAA_X 2)

# File Settings
set(HEADERS 
	lib/include
	include/
	include/render
	include/scene
	include/scene/procedural
	include/scene/loaded
	include/input
)

file(GLOB SOURCES
	lib/include/GL/gl3w.c
	src/*.cpp
	src/render/*.cpp
	src/scene/*.cpp
	src/scene/procedural/*.cpp
	src/scene/loaded/*.cpp
	src/input/*.cpp
)

# Levels to compile with
set(LEVELS
	levels/one.level
)

# Shaders to compile with
set(SHADERS
	shaders/phong.frag
	shaders/phong.vert
	shaders/depth.frag
	shaders/depth.vert
)

# Debug Settings
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	add_compile_definitions(
		#DEBUG_CAMERA
		#DEBUG_LINES
		DEBUG_GL
	)
endif()

##############################
#         CMAKE WORK         #
##############################
# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Name project
project (${PROJECT_NAME})

# Add source files
add_executable (${PROJECT_NAME} ${SOURCES})
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Graphics PROPERTY CXX_STANDARD 20)
endif()

# Include project headers
foreach(HEADER IN LISTS HEADERS)
	target_include_directories(${PROJECT_NAME} PRIVATE ${HEADER})
endforeach()

# Compile GLFW with source
add_subdirectory(lib/glfw-3.4)
target_link_libraries(${PROJECT_NAME} glfw)

# Compile GLM with source
add_subdirectory(lib/glm)
target_link_libraries(${PROJECT_NAME} glm::glm)

# Compile ASSIMP with source
add_subdirectory(lib/assimp-5.4.1)
target_link_libraries(${PROJECT_NAME} assimp::assimp)

# Copy level files to binary directory
foreach(LEVEL ${LEVELS})
	configure_file(${CMAKE_SOURCE_DIR}/${LEVEL} ${CMAKE_BINARY_DIR}/${LEVEL} COPYONLY)
endforeach()

# Copy shader files to binary directory
foreach(SHADER ${SHADERS})
	configure_file(${CMAKE_SOURCE_DIR}/${SHADER} ${CMAKE_BINARY_DIR}/${SHADER} COPYONLY)
endforeach()

# Copy models
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/models ${CMAKE_BINARY_DIR}/models)

# Definitions
add_compile_definitions(
	# Select NONE|NVIDIA|AMD depending on the system's discrete GPU
	${TARGET_GPU}

	# Add directory definitions
	SHADER_DIR="./shaders/"
	MODEL_DIR="./models/"
	LEVEL_DIR="./levels/"

	# Add anti-aliasing definition
	MULTISAMPLE_NUM=${MSAA_X}

	# Add dimension definitions
	WINDOW_WIDTH=${DEFAULT_WINDOW_WIDTH}
	WINDOW_HEIGHT=${DEFAULT_WINDOW_HEIGHT}
)
