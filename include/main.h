#pragma once

#include <cstdio>
#include <string>

#include "render.h"
#include "shader_manager.h"
#include "scene.h"
#include "keyboard.h"
#include "camera.h"
#include "shadow.h"
#include "light.h"

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
