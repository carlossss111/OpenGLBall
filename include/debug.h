#ifdef DEBUG_GL
#pragma once

#include "render.h"
#include "scene.h"

namespace Debug {
    void onLoop(Renderer* renderer, Scene* scene);
}

#endif