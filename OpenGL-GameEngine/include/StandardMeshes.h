#pragma once

#include "Mesh.h"
#include <memory>

class StandardMeshes {
public:
    // flat shaded 1x1x1 cube
    static std::shared_ptr<Mesh> CreateCube(float uvScale = 1.0f);

    // smooth shaded sphere with adjustable resolution of diameter 1.0
    static std::shared_ptr<Mesh> CreateSphere(int sectors = 36, int stacks = 18);

    // flat plane of dimensions 100 x 1 x 100
    static std::shared_ptr<Mesh> CreatePlane(float uvScale = 1.0f);

    // overlay mesh
    static std::shared_ptr<Mesh> CreateOverlay();
};