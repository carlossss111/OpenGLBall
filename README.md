# So-far-unnamed Graphics Project

The aim of this project is to use OpenGL to render a ball, a platform and maybe some scenery.
The level should tilt to roll the ball, like the game Super Monkey Ball.
It would be cool to add PBR and point shadows and stuff too. Let's see how it goes.

## Compiling
The project is organised with CMake version >= 3.22. It can be compiled with gcc or msvc.
The executable will be built as `${project_root}/build/Graphics[.exe]`.

### Windows
Open in Visual Studio 2022 and press F7 ('Build All'). 

### Linux
```
mkdir build && cd build
cmake ..
make -j 8 
```

## Usage
None so far. WIP.

## Libraries
Libraries are built from source in `lib/`.
* glfw-3.4 Window API
* gl3w-3a3327 OpenGL API
* glm-0af5cc Matrix Maths
* khr-platform-67a3e0 Types and Definitions
* stb-image-2.29 Image Reader
* assimp-5.4.1 Model & Mesh Loader
* physx-5.4.0 Nvidia Physics Engine
OpenGL drivers are required for the target GPU.
