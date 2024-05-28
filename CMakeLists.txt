cmake_minimum_required (VERSION 3.8)

##############################
#      PROJECT SETTINGS      #
##############################
set(PROJECT_NAME "Graphics")
set(TARGET_GPU "NVIDIA")
set(CXX_STD 20)
set(MSAA_X 2)

set(HEADERS 
	lib/include
	include/
	include/render
	include/objects
)

file(GLOB SOURCES
	lib/include/GL/gl3w.c
	src/*.cpp
	src/render/*.cpp
	src/objects/*.cpp
)

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
  set_property(TARGET Graphics PROPERTY CXX_STANDARD ${CXX_STD})
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

# Select NONE|NVIDIA|AMD depending on the system's discrete GPU.
add_compile_definitions(${TARGET_GPU})

# Add shader directory definition
add_compile_definitions(SHADER_DIR="${CMAKE_SOURCE_DIR}/shaders/")

# Add anti-aliasing definition
add_compile_definitions(MULTISAMPLE_NUM=${MSAA_X})
