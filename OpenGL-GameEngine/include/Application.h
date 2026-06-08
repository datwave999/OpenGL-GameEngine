#pragma once

#include <vector>
#include <memory>
#include "AssetContainer.h"
#include "State.h"

// Forward Declarations
class Window;

class Application {
public:
    Application();
    ~Application();

    // --- Core Lifecycle ---
    bool Initialize();
    void Run();
    void Shutdown();

    // --- State Management ---
    void PushState(std::unique_ptr<State> state);
    void PopState();

    // --- Accessors for States ---
    Window* GetWindow() const { return window.get(); }
    AssetContainer& GetAssets() { return assets; }

private:
    // --- Internal Loop ---
    void Update(double dt);
    void Render();

    // --- Time & FPS ---
    int frameCount;
    double fpsTimer;
    double lastFrameTime;

    // --- Core Systems ---
    std::unique_ptr<Window> window;
    AssetContainer assets;

    // --- State Stack ---
    std::vector<std::unique_ptr<State>> states;
};