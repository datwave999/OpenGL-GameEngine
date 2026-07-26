#pragma once
#include <entt/entt.hpp>
#include "Resources/AssetManager.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/RenderComponent.h"
#include "ECS/Components/LightComponents.h"

struct RenderCommand {
    uint32_t shaderHandle;
    uint32_t materialHandle;
    uint32_t meshHandle;
    glm::mat4 modelMatrix;
    glm::mat3 normalMatrix;
};

class RenderSystem {
public:
    void Render(entt::registry& registry, AssetManager& assets, glm::vec3 cameraPos);

private:
    std::vector<RenderCommand> renderQueue;
};