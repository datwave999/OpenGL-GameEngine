#include "PauseState.h"
#include "Application.h"
#include "Window.h"
#include "Input.h"
#include "Shader.h"
#include "Mesh.h"
#include "StandardMeshes.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

PauseState::PauseState()
{
}

PauseState::~PauseState()
{
}

void PauseState::Initialize(Application* app) {
    // 1. Free the mouse cursor
    glfwSetInputMode(app->GetWindow()->getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // 2. get the asset container from app
    AssetContainer& assets = app->GetAssets();

    // 2. Load the overlay shader
    overlayShader = assets.getShader("overlayShader", "assets/Shaders/overlay.vert", "assets/Shaders/overlay.frag");

    // 3. Create Fullscreen Mesh
    overlayMesh = assets.getMesh("overlayMesh", StandardMeshes::CreateOverlay());
}

void PauseState::Update(Application* app, float dt) {
    // Unpause logic
    if (Input::keyWentDown(GLFW_KEY_ESCAPE)) {
        glfwSetInputMode(app->GetWindow()->getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        app->PopState();
    }
}

void PauseState::Render(Application* app) {
    // 1. Setup OpenGL state for 2D UI rendering
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    // 2. Draw using your Mesh class!
    overlayShader->enableShader();

    overlayMesh->RenderMesh();

    overlayShader->disableShader();

    // 3. Restore OpenGL state 
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}
