#include "PlayState.h"
#include "PauseState.h" // To push onto stack when we pause
#include "Application.h"
#include "Window.h"
#include "Camera.h"
#include "Object.h"
#include "Shader.h"
#include "Input.h"
#include "StandardMeshes.h"
#include "Material.h"
#include <iostream>

PlayState::PlayState()
{
}

PlayState::~PlayState()
{
}

void PlayState::Initialize(Application* app) {
    // 1. Setup Input & Camera
    glfwSetInputMode(app->GetWindow()->getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    camera = std::make_unique<Camera>();

    // 3. Grab AssetManager from Application
    AssetContainer& assets = app->GetAssets();

    // 2. Compile Shaders 
    coreShader = assets.getShader("coreShader", "assets/Shaders/core.vert", "assets/Shaders/core.frag");
    overlayShaderPreload = assets.getShader("overlayShader", "assets/Shaders/overlay.vert", "assets/Shaders/overlay.frag");

    // 4. Register Meshes
    auto cubeMesh = assets.getMesh("cube", StandardMeshes::CreateCube(2.0f));
    auto sphereMesh = assets.getMesh("sphere", StandardMeshes::CreateSphere());
    auto planeMesh = assets.getMesh("plane", StandardMeshes::CreatePlane(10.0f));

    // 5. Get Textures 
    auto obamaTex = assets.getTexture("obama", "assets/Textures/obama_sandwich.jpg");
    auto flagTex = assets.getTexture("flag", "assets/Textures/community.png");
    auto greyPrototypeTex = assets.getTexture("prototypeGrey", "assets/Textures/PNG/Light/texture_08.png");

    // 6. Get Materials
    auto obamaMat = assets.getMaterial("obamaSandwich", obamaTex, 0);
    auto flagMat = assets.getMaterial("communityFlag", flagTex, 0);
    auto greyMat = assets.getMaterial("greyFloor", greyPrototypeTex, 0);

    // 7. Get Models
    auto obamaCubeModel = assets.getModel("obamaCube", cubeMesh, obamaMat);
    auto flagCubeModel = assets.getModel("flagCube", cubeMesh, flagMat);
    auto sandwichSphereModel = assets.getModel("sandwichSphere", sphereMesh, obamaMat);
    auto floorModel = assets.getModel("floor", planeMesh, greyMat);
    auto sedan = assets.getModel("sedan", "assets/Models/sedan/sedan.obj", false);
    auto race = assets.getModel("race", "assets/Models/race/race.obj", false);

    // 8. Create Objects 
    objects.push_back(std::make_unique<Object>(obamaCubeModel));
    objects.push_back(std::make_unique<Object>(flagCubeModel));
    objects.push_back(std::make_unique<Object>(sandwichSphereModel));
    objects.push_back(std::make_unique<Object>(sedan));
    objects.push_back(std::make_unique<Object>(race));
    objects.push_back(std::make_unique<Object>(floorModel));

    // 9. Initial Object Setup
    objects[1]->transform.SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));
    objects[2]->transform.SetPosition(glm::vec3(0.0f, -1.0f, 3.0f));
    objects[3]->transform.SetPosition(glm::vec3(2.0f, 0.0f, 0.0f));
    objects[4]->transform.SetPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
    objects[5]->transform.SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));
    objects[5]->transform.SetScale(glm::vec3(100.0f, 0.1f, 100.0f));
}

void PlayState::Update(Application* app, double dt) {
    // --- PAUSE LOGIC ---
    if (Input::keyWentDown(GLFW_KEY_ESCAPE)) {
        app->PushState(std::make_unique<PauseState>());
        return; 
    }

    // --- GAME LOGIC ---
    objects[1]->transform.Rotate(40 * (float)dt, glm::vec3(0.0f, 0.0f, 1.0f));
    objects[2]->transform.Rotate(50 * (float)dt, glm::vec3(0.0f, 1.0f, 0.0f));

    objects[4]->transform.MoveRelative(glm::vec3(0.0f, 0.0f, 2.0f) * float(dt));
    objects[4]->transform.Rotate(30 * (float)dt, glm::vec3(0.0f, 1.0f, 0.0f));

    // --- UPDATE CAMERA ---
    camera->Update(dt);
    camera->processMouseScroll(Input::getScrollDY());
}

void PlayState::Render(Application* app) {
    coreShader->enableShader();

    camera->setUniforms(coreShader.get(), app->GetWindow()->getWidth(), app->GetWindow()->getHeight());

    for (const auto& obj : objects) {
        obj->Render(coreShader.get());
    }

    coreShader->disableShader();
}
