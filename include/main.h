#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <string>

#include "render.h"
#include "shader.h"
#include "scene.h"
#include "keyboard.h"
#include "camera.h"

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
