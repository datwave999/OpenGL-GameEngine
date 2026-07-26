#pragma once

#include "Core/State.h"
#include <vector>
#include <memory>
#include <cstdint>

#include <entt/entt.hpp>
#include "ECS/Entity.h"
#include "ECS/Systems/TransformSystem.h"
#include "ECS/Systems/LightSystem.h"
#include "ECS/Systems/RenderSystem.h"

class Camera;
class Shader;

class PlayState : public State {
public:
    PlayState(Application* app);
    ~PlayState() override;

    // State Interface Overrides
    void Initialize() override;
    void Update(float dt) override;
    void Render() override;
    bool isOpaque() const override { return true; }

private:
    // --- Game Systems ---
    std::unique_ptr<Camera> camera;
    
    // ECS
    entt::registry registry;
    TransformSystem transformSystem;
    LightSystem lightSystem;
    RenderSystem renderSystem;

    // Shaders
    uint32_t coreShaderHandle;
    uint32_t unlitShaderHandle;
    uint32_t overlayShaderHandle;

    // entities we want to animate in Update() are stored
    std::unordered_map<std::string, Entity> entities;
};