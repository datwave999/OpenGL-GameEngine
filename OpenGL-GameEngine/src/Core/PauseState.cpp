#include "Core/PauseState.h"
#include "Core/Application.h"
#include "Core/Window.h"
#include "Core/Input.h"
#include "Graphics/Shader.h"
#include "Resources/AssetData.h"
#include "Resources/StandardMeshes.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

PauseState::PauseState(Application* app) : State(app) {}

PauseState::~PauseState() {}

void PauseState::Initialize() {
    // 1. Free the mouse cursor
    glfwSetInputMode(app->GetWindow()->getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    AssetManager& assets = app->GetAssets();
    
    // 2. Load the overlay shader and mesh
    overlayShaderHandle = assets.LoadShader("overlay", "assets/Shaders/overlay.vert", "assets/Shaders/overlay.frag");
    overlayMeshHandle = assets.CreateMesh("overlayMesh", StandardMeshes::CreateOverlay());
}

void PauseState::Update(float dt) {
    // Unpause logic
    if (Input::keyWentDown(GLFW_KEY_ESCAPE)) {
        glfwSetInputMode(app->GetWindow()->getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        app->PopState();
    }
}

void PauseState::Render() {
    // 1. Setup OpenGL state for 2D UI rendering
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    AssetManager& assets = app->GetAssets();
    Shader* shader = assets.GetShader(overlayShaderHandle);
    const MeshGPU* mesh = assets.GetMesh(overlayMeshHandle);

    if (shader && mesh) {
        shader->enableShader();

        // 2. Draw the raw GPU mesh manually
        glBindVertexArray(mesh->VAO);
        glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        shader->disableShader();
    }

    // 3. Restore OpenGL state 
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}
