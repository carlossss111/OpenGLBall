#pragma once

#include <cstdio>
#include <string>

#include "render/render.h"
#include "render/shader_manager.h"
#include "render/shadow.h"
#include "render/light.h"

#include "scene/scene.h"

#include "input/keyboard.h"
#include "input/camera.h"

#include "physics/physics.h"

#ifdef DEBUG_GL
#include "debug.h"
#endif

#ifdef NVIDIA
extern "C" {
	__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
}
#endif

#ifdef AMD
extern "C" {
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif
