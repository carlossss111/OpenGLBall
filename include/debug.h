#ifdef DEBUG_GL
#pragma once

#include "render/render.h"
#include "scene/scene.h"

namespace Debug {
    void onLoop(Renderer* renderer, Scene* scene);
}

#endif