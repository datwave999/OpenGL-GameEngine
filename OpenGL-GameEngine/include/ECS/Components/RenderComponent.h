#pragma once

#include <cstdint>

struct RenderComponent {
    uint32_t modelHandle = UINT32_MAX;  // Index into AssetManager's model vector
};