cmake_minimum_required (VERSION 3.22)

##############################
#      PROJECT SETTINGS      #
##############################
# Main Settings
set(PROJECT_NAME "Graphics")
set(TARGET_GPU "HYBRID")
set(DEFAULT_WINDOW_WIDTH 640)
set(DEFAULT_WINDOW_HEIGHT 480)
set(MSAA_X 2)

# File Settings
set(HEADERS 
	lib/include
	include/
)

file(GLOB SOURCES
	lib/include/GL/gl3w.c
	src/*.cpp
	src/render/*.cpp
	src/scene/*.cpp
	src/scene/procedural/*.cpp
	src/scene/loaded/*.cpp
	src/input/*.cpp
	src/input/camera_types/*.cpp
	src/physics/*.cpp
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
	shaders/line.frag
	shaders/line.vert
	shaders/skybox.vert
	shaders/skybox.frag
)

# Debug Settings
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	add_compile_definitions(
		DEBUG_GL # REQUIRED for most of the below options

		DEBUG_CAMERA
		#DEBUG_CAMERA_POSITION
		#DEBUG_LINES
		DEBUG_PHYSX
		#DEBUG_TILT
		#DEBUG_FRAMERATE
		#DEBUG_SLOWMOTION
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
if(${CMAKE_BUILD_TYPE} MATCHES "Release")
	set(ASSIMP_WARNINGS_AS_ERRORS OFF)
endif()
set(ASSIMP_INSTALL OFF)
set(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(lib/assimp-5.4.1)
target_link_libraries(${PROJECT_NAME} assimp::assimp)

# Compile NVidia PhysX with source
if(LINUX)
	set(TARGET_BUILD_PLATFORM "linux") # has to match the TARGET_BUILD_PLATFORM in $ENV{PHYSX_PATH}/physix/buildtools/cmake_generate_projects.py
	set(NV_USE_STATIC_WINCRT OFF CACHE BOOL "Use the statically linked windows CRT")
	set(NV_USE_DEBUG_WINCRT OFF CACHE BOOL "Use the debug version of the CRT")
elseif(WIN32)
	set(TARGET_BUILD_PLATFORM "windows")
	set(NV_USE_STATIC_WINCRT ON CACHE BOOL "Use the statically linked windows CRT")
	set(NV_USE_DEBUG_WINCRT ON CACHE BOOL "Use the debug version of the CRT")
endif()

set(PHYSX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/physx-5.4.0)
set(PHYSX_ROOT_DIR ${PHYSX_PATH} ) #This is needed for $ENV{PHYSX_PATH}/compiler/public/CMakeLists.txt
set(PHYSX_INCLUDE_DIRS {PHYSX_PATH}/include/ {PHYSX_PATH}/../pxshared/include/ )
set(PHYSX_LIBRARIES 
	PhysX
	PhysXCommon
	PhysXFoundation
	PhysXCooking
	PhysXExtensions
)

set(PX_BUILDSNIPPETS OFF CACHE BOOL "Generate the snippets")
set(PX_BUILDPUBLICSAMPLES OFF CACHE BOOL "Generate the samples projects")
set(PX_GENERATE_STATIC_LIBRARIES ON CACHE BOOL "Generate static libraries")
set(PX_FLOAT_POINT_PRECISE_MATH OFF CACHE BOOL "Float point precise math")
if(${PX_GENERATE_STATIC_LIBRARIES} MATCHES ON)
	add_compile_definitions(PX_PHYSX_STATIC_LIB)
endif()

set(PXSHARED_PATH ${PHYSX_PATH}/../pxshared)
set(PXSHARED_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

set(CMAKEMODULES_VERSION "1.27")
set(CMAKEMODULES_PATH ${PHYSX_PATH}/../externals/cmakemodules)
set(PX_OUTPUT_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/lib/physx-5.4.0/generated)
set(PX_OUTPUT_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/lib/physx-5.4.0/generated)
add_subdirectory(${PHYSX_PATH}/compiler/public)
include_directories(${PHYSX_PATH}/include)
target_link_libraries(${PROJECT_NAME} ${PHYSX_LIBRARIES})

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
	# Select INTEL|NVIDIA|AMD depending on the system's discrete GPU
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
