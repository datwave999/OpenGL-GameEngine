#pragma once

#include "Mesh.h"

class StandardMeshes {
public:
    // flat shaded 1x1x1 cube
    static Mesh* CreateCube();

    // smooth shaded sphere with adjustable resolution of diameter 1.0
    static Mesh* CreateSphere(int sectors = 36, int stacks = 18);
};