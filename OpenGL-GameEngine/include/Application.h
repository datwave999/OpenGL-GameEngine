#pragma once

#include <string>
#include<vector>
#include<unordered_map>
#include <memory>

#include <glm/glm.hpp>

#include "AssetContainer.h"

// Forward Declarations
class Window;
class Shader;
class Mesh;
class Texture;
class Camera;
class Object;
class Material;


class Application {
public:
    Application();
    ~Application();

    // --- Core Lifecycle ---
    bool Initialize();
    void Run();
    void Shutdown();

private:
    // --- Internal Loop ---
    void Update(double dt);
    void Render();

    // --- FPS ---
    int frameCount;
    double fpsTimer;

    // --- Core Systems ---
    std::unique_ptr<Window> window;
    std::unique_ptr<Camera> camera;

    // --- Assets ---
    Shader* coreShader;
    AssetContainer assets;
    std::vector<std::unique_ptr<Object>> objects;

    // --- Engine State ---
    double lastFrameTime;
};
