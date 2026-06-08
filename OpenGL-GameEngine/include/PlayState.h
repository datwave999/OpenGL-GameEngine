#pragma once

#include "State.h"
#include <vector>
#include <memory>

class Camera;
class Object;
class Shader;

class PlayState : public State {
public:
    PlayState();
    ~PlayState() override;

    // State Interface Overrides
    void Initialize(Application* app) override;
    void Update(Application* app, double dt) override;
    void Render(Application* app) override;

private:
    // --- Game Systems ---
    std::unique_ptr<Camera> camera;
    std::vector<std::unique_ptr<Object>> objects;
    std::shared_ptr<Shader> coreShader;

    // --- Preloads ---
    std::shared_ptr<Shader> overlayShaderPreload;
};