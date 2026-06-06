#pragma once

#include <string>
#include <glm/glm.hpp>

// Forward Declarations
class Window;
class Shader;
class Mesh;
class Texture;

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

    // --- Core Systems ---
    Window* window;

    // --- Assets ---
    Shader* coreShader;
    Mesh* square;
    Texture* obama;
    Texture* flag;

    // --- Engine State ---
    glm::mat4 model;
    double lastFrameTime;
};
