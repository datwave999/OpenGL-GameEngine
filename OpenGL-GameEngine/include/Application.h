#pragma once

#include <string>
#include <glm/glm.hpp>

// Forward Declarations
class Window;
class Shader;
class Mesh;
class Texture;
class Camera;

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
    Window* window;
    Camera* camera;

    // --- Assets ---
    Shader* coreShader;
    Mesh* cube;
    Texture* obama;
    Texture* flag;

    // --- Engine State ---
    glm::mat4 model;
    double lastFrameTime;
};
