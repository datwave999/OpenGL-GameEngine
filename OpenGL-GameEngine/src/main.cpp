#include "Core/Application.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

int main() {
    // Memory Leaks check
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    Application app;

    if (app.Initialize()) {
        app.Run();
    }

    return 0; 
}

// OOP = bad, fix the architecture! DOD and ECS
// emissive mapping
// HDR support and then glow effect using gussian blur
// use core.frag for light objects aswell using the new emissive maps 
// use SSBO to upload material data to the gpu
// decouple the Mesh from the Model iteration. Group your objects by Material. Bind the Tree Material exactly once, and then draw all 10 tree meshes in a row.
// Forward+ (Tiled Light Culling)
// Normal mapping
// shadow mapping
// skybox
// Release 1.0.0

// transition to PBR
// Deferred shading