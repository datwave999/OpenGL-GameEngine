#include "Application.h"

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