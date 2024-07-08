# GlBall Graphics & Physics Project

The aim of this project is to use OpenGL to render a ball, a platform and maybe some scenery.
The level should tilt to roll the ball, like the game Super Monkey Ball.
It would be cool to add PBR and point shadows and stuff too. Let's see how it goes.

## Compiling
The project is organised with CMake version >= 3.22. It can be compiled with gcc or msvc.
The executable will be built as `${project_root}/build/Graphics[.exe]`.

The code primarily developed on a Linux system and is occasionally tested on a Windows VM.

### Windows
Open in Visual Studio 2022 with the C++ extension installed and press F7 ('Build All'). 

### Linux
```
mkdir build && cd build
cmake ..
make -j 8 
```

## Progress so far
WASD to tilt the board. Arrow keys to move the camera. Q to reset the ball (to-do: fix needing to do this after starting), Space to move the position of the lightsource to the camera, C to change between follow and stage camera.

![Follow GIF](demo1.gif)
![Stage GIF](demo2.gif)

(lag is from recording)

* Phong lighting, shadows, blending & skybox
* Level loader and editor
* Camera Controls and debug view
* Ball physics with per-triangle collisions
* Board tilt matrix calculations
* and more in the works!


## Libraries
Libraries are built from source in `lib/`.
* glfw-3.4 Window API
* gl3w-3a3327 OpenGL API
* glm-0af5cc Matrix Maths
* khr-platform-67a3e0 Types and Definitions
* stb-image-2.29 Image Reader
* assimp-5.4.1 Model & Mesh Loader
* physx-5.4.0 Nvidia Physics Engine

OpenGL drivers are also required, they are probably already installed.
