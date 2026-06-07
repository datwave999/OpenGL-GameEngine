#pragma once

#include "Mesh.h"

class StandardMeshes {
public:
    // Generates a sharp, flat-shaded 1x1x1 cube
    static Mesh* CreateCube();

    // Generates a smooth-shaded sphere with adjustable resolution
    static Mesh* CreateSphere(int sectors = 36, int stacks = 18);
};