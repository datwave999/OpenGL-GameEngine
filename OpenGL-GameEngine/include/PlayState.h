#pragma once

#include "State.h"
#include <vector>
#include <memory>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "LightManager.h"

class Camera;
class Object;
class Shader;
class LightObject;


class PlayState : public State {
public:
    PlayState();
    ~PlayState() override;

    // State Interface Overrides
    void Initialize(Application* app) override;
    void Update(Application* app, double dt) override;
    void Render(Application* app) override;
    bool isOpaque() const override { return true; }

private:
    // --- Game Systems ---
    std::unique_ptr<Camera> camera;
    std::vector<std::unique_ptr<Object>> objects;
    std::shared_ptr<Shader> coreShader;

    // --- Lighting ---
    LightManager lights;

    std::vector<std::unique_ptr<LightObject>> lightObjects;
    std::shared_ptr<Shader> unlitShader;

    // --- Preloads ---
    std::shared_ptr<Shader> overlayShaderPreload;
};