#include "ECS/Systems/RenderSystem.h"
#include "Graphics/Uniform.h"
#include <algorithm>
#include <vector>

void RenderSystem::Render(entt::registry& registry, AssetManager& assets, glm::vec3 cameraPos) {

    renderQueue.clear();
    auto view = registry.view<RenderComponent, TransformComponent>();

    // 1. Extraction
    view.each([&](auto entity, const auto& rc, const auto& tc) {
        const ModelAsset* model = assets.GetModel(rc.modelHandle);
        if (!model) return;

        for (const auto& node : model->nodes) {
            const MaterialAsset* mat = assets.GetMaterial(node.materialHandle);

            RenderCommand cmd;
            cmd.shaderHandle = mat->shaderHandle;
            cmd.materialHandle = node.materialHandle;
            cmd.meshHandle = node.meshHandle;
            cmd.modelMatrix = tc.modelMatrix;
            cmd.normalMatrix = tc.normalMatrix;
            renderQueue.push_back(cmd);
        }
    });

    // 2. Sorting (By Shader first then Material)
    std::sort(renderQueue.begin(), renderQueue.end(), 
        [](const RenderCommand& a, const RenderCommand& b) {
        if (a.shaderHandle != b.shaderHandle) return a.shaderHandle < b.shaderHandle;
        return a.materialHandle < b.materialHandle;
        }
    );

    // 3. Execution
    uint32_t currentShaderID = -1;
    uint32_t currentMaterial = -1;
    uint32_t currentMesh = -1;
    Shader* activeShader = nullptr;

    for (const auto& cmd : renderQueue) {
        // A. Bind Shader if changed
        if (cmd.shaderHandle != currentShaderID) {
            activeShader = assets.GetShader(cmd.shaderHandle);
            activeShader->enableShader();
            activeShader->setUniform(Uniform::cameraPos, cameraPos);
            currentShaderID = cmd.shaderHandle;
        }

        // B. Per-Object Uniforms
        if (activeShader == nullptr) continue;
        activeShader->setUniform(Uniform::model, cmd.modelMatrix);
        activeShader->setUniform(Uniform::normalMatrix, cmd.normalMatrix);

        // C. Bind Material properties if changed
        if (cmd.materialHandle != currentMaterial) {
            const MaterialAsset* mat = assets.GetMaterial(cmd.materialHandle);

            glActiveTexture(GL_TEXTURE0 + mat->diffuseUnit);
            glBindTexture(GL_TEXTURE_2D, assets.GetTexture(mat->diffuseTextureHandle)->textureID);
            activeShader->setUniform(Uniform::materialDiffuse, mat->diffuseUnit);

            glActiveTexture(GL_TEXTURE0 + mat->specularUnit);
            glBindTexture(GL_TEXTURE_2D, assets.GetTexture(mat->specularTextureHandle)->textureID);
            activeShader->setUniform(Uniform::materialSpecular, mat->specularUnit);

            glBindBufferBase(GL_UNIFORM_BUFFER, 2, mat->materialUBO);

            currentMaterial = cmd.materialHandle;
        }

        // D. Bind Mesh (VAO) if changed
        if (cmd.meshHandle != currentMesh) {
            const MeshGPU* mesh = assets.GetMesh(cmd.meshHandle);
            glBindVertexArray(mesh->VAO);
            currentMesh = cmd.meshHandle;
        }

        // E. The Draw Call
        glDrawElements(GL_TRIANGLES, assets.GetMesh(cmd.meshHandle)->indexCount, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}