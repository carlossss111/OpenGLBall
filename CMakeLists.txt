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
	include/objects
	include/objects/procedural
	include/objects/loaded
	include/input
)

file(GLOB SOURCES
	lib/include/GL/gl3w.c
	src/*.cpp
	src/render/*.cpp
	src/objects/*.cpp
	src/objects/procedural/*.cpp
	src/objects/loaded/*.cpp
	src/input/*.cpp
)

# Debug Settings
set(DEBUG_GL 1)
set(DEBUG_CAMERA 0)

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

# Select NONE|NVIDIA|AMD depending on the system's discrete GPU.
add_compile_definitions(${TARGET_GPU})

# Add directory definitions
add_compile_definitions(SHADER_DIR="${CMAKE_SOURCE_DIR}/shaders/")
add_compile_definitions(MODEL_DIR="${CMAKE_SOURCE_DIR}/models/")
add_compile_definitions(LEVEL_DIR="${CMAKE_SOURCE_DIR}/levels/")

# Add anti-aliasing definition
add_compile_definitions(MULTISAMPLE_NUM=${MSAA_X})

# Add dimension definitions
add_compile_definitions(WINDOW_WIDTH=${DEFAULT_WINDOW_WIDTH})
add_compile_definitions(WINDOW_HEIGHT=${DEFAULT_WINDOW_HEIGHT})

# Add debug definitions
add_compile_definitions(DEBUG_CAMERA=${DEBUG_CAMERA})
add_compile_definitions(DEBUG_GL=${DEBUG_GL})