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

#ifdef DEBUG_SLOWMOTION
#define TIME_MOD 0.5f
#else
#define TIME_MOD 1.f
#endif

#ifndef DEBUG_FRAMERATE
#define updateDt(deltaTime)                                 \
    static float lastFrame = 0.f;                           \
    float currentFrame = glfwGetTime();                     \
    deltaTime = (currentFrame - lastFrame) * TIME_MOD;      \
    lastFrame = currentFrame
#else
#define updateDt(deltaTime)                                 \
    static float lastFrame = 0.f;                           \
    float currentFrame = glfwGetTime();                     \
    deltaTime = (currentFrame - lastFrame) * TIME_MOD;      \
    lastFrame = currentFrame;                               \
    printf("Delta: %f, FPS: %.0f\n", deltaTime, 1/deltaTime)
#endif
