cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Name Project
project ("Graphics")

# Add source to this project's executable.
add_executable (Graphics "main.cpp" "main.h")
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Graphics PROPERTY CXX_STANDARD 20)
endif()

# Compile GLFW with source
add_subdirectory(lib/glfw-3.4)
target_link_libraries("Graphics" glfw)

# Find OpenGL on the system and link
find_package(OpenGL REQUIRED)
target_link_libraries("Graphics" OpenGL::GL)

# Compile GLM with source
add_subdirectory(lib/glm)
target_link_libraries("Graphics" glm::glm)

# Include header-only libraries, e.g. stb_image
target_include_directories("Graphics" PRIVATE lib)
