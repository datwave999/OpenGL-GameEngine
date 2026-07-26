#include "Core/PlayState.h"
#include "Core/PauseState.h" // To push onto stack when we pause
#include "Core/Application.h"
#include "Core/Window.h"
#include "Core/Camera.h"
#include "Graphics/Shader.h"
#include "Core/Input.h"
#include "Resources/StandardMeshes.h"
#include <iostream>

PlayState::PlayState(Application* app) : State(app) {}

PlayState::~PlayState() {}

void PlayState::Initialize() {
    // 1. Setup Input & Camera
    glfwSetInputMode(app->GetWindow()->getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    camera = std::make_unique<Camera>();

    // 2. Grab AssetManager from Application
    AssetManager& assets = app->GetAssets();

    // 3. Compile Shaders 
    coreShaderHandle = assets.LoadShader("core", "assets/Shaders/core.vert", "assets/Shaders/core.frag");
    unlitShaderHandle = assets.LoadShader("unlit", "assets/Shaders/unlit.vert", "assets/Shaders/unlit.frag");
    overlayShaderHandle = assets.LoadShader("overlay", "assets/Shaders/overlay.vert", "assets/Shaders/overlay.frag");

    // 4. Get Meshes
    auto cubeMesh = assets.CreateMesh("cube", StandardMeshes::CreateCube(2.0f));
    auto sphereMesh = assets.CreateMesh("sphere", StandardMeshes::CreateSphere());
    auto planeMesh = assets.CreateMesh("plane", StandardMeshes::CreatePlane(10.0f));

    // 5. Get Textures 
    auto obamaTex = assets.LoadTexture("obama", "assets/Textures/obama_sandwich.jpg");
    auto flagTex = assets.LoadTexture("flag", "assets/Textures/community.png");
    auto greyPrototypeTex = assets.LoadTexture("prototypeGrey", "assets/Textures/PNG/Light/texture_08.png");
    auto blueTex = assets.CreateTexture("blue", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    auto redTex = assets.CreateTexture("red", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    auto greenTex = assets.CreateTexture("green", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

    // 6. Get Materials
    auto obamaMat = assets.CreateMaterial("obamaSandwich", obamaTex, UINT32_MAX, 64.0f, 1.0f, coreShaderHandle);
    auto flagMat = assets.CreateMaterial("communityFlag", flagTex, UINT32_MAX, 16.0f, 0.6f, coreShaderHandle);
    auto greyMat = assets.CreateMaterial("greyFloor", greyPrototypeTex, UINT32_MAX, 16.0f, 0.6f, coreShaderHandle);

    auto blueMat = assets.CreateMaterial("blueMat", blueTex, UINT32_MAX, 1.0f, 1.0f, unlitShaderHandle);
    auto redMat = assets.CreateMaterial("redMat", redTex, UINT32_MAX, 1.0f, 1.0f, unlitShaderHandle);
    auto greenMat = assets.CreateMaterial("greenMat", greenTex, UINT32_MAX, 1.0f, 1.0f, unlitShaderHandle);

    // 7. Models
    auto obamaModel = assets.CreateModel("obamaCube", cubeMesh, obamaMat);
    auto flagModel = assets.CreateModel("flagCube", cubeMesh, flagMat);
    auto sphereModel = assets.CreateModel("sandwichSphere", sphereMesh, obamaMat);
    auto floorModel = assets.CreateModel("floor", planeMesh, greyMat);

    auto sedanModel = assets.LoadModel("sedan", "assets/Models/sedan/sedan.obj", coreShaderHandle);
    auto raceModel = assets.LoadModel("race", "assets/Models/race/race.obj", coreShaderHandle);
    auto backpackModel = assets.LoadModel("backpack", "assets/Models/backpack/backpack.obj", coreShaderHandle, true);
    auto blueLightModel = assets.CreateModel("blueLight", sphereMesh, blueMat);
    auto redLightModel = assets.CreateModel("redLight", sphereMesh, redMat);
    auto greenLightModel = assets.CreateModel("greenLight", sphereMesh, greenMat);
    auto flashLightModel = assets.LoadModel("flashlight", "assets/Models/flashlight/silencer-small.fbx", unlitShaderHandle);

    // The Sun
    lightSystem.SetDirectionalLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.8f), 0.5f, 0.2f);

    // 6. Create Entities
    entities["obamaCube"] = Entity(&registry);
    entities["obamaCube"].AddComponent<TransformComponent>(glm::vec3(0.0f, 0.0f, 0.0f));
    entities["obamaCube"].AddComponent<RenderComponent>(obamaModel);

    entities["flag"] = Entity(&registry);
    entities["flag"].AddComponent<TransformComponent>(glm::vec3(0.0f, 2.0f, 0.0f));
    entities["flag"].AddComponent<RenderComponent>(flagModel);

    entities["sphere"] = Entity(&registry);
    entities["sphere"].AddComponent<TransformComponent>(glm::vec3(0.0f, -1.0f, 3.0f));
    entities["sphere"].AddComponent<RenderComponent>(sphereModel);

    entities["sedan"] = Entity(&registry);
    entities["sedan"].AddComponent<TransformComponent>(glm::vec3(2.0f, 0.0f, 0.0f));
    entities["sedan"].AddComponent<RenderComponent>(sedanModel);

    entities["race"] = Entity(&registry);
    entities["race"].AddComponent<TransformComponent>(glm::vec3(-2.0f, 0.0f, 0.0f));
    entities["race"].AddComponent<RenderComponent>(raceModel);

    entities["floor"] = Entity(&registry);
    entities["floor"].AddComponent<TransformComponent>(glm::vec3(0.0f, -1.0f, 0.0f), glm::quat(1, 0, 0, 0), glm::vec3(100.0f, 0.1f, 100.0f));
    entities["floor"].AddComponent<RenderComponent>(floorModel);

    entities["backpack"] = Entity(&registry);
    entities["backpack"].AddComponent<TransformComponent>(glm::vec3(0.0f, 3.0f, 10.0f));
    entities["backpack"].AddComponent<RenderComponent>(backpackModel);

    entities["blueLight"] = Entity(&registry);
    entities["blueLight"].AddComponent<TransformComponent>(glm::vec3(0.0f, 2.0f, 0.0f), glm::quat(1, 0, 0, 0), glm::vec3(0.2f));
    entities["blueLight"].AddComponent<RenderComponent>(blueLightModel);
    entities["blueLight"].AddComponent<PointLightComponent>(glm::vec3(0.0f, 0.0f, 1.0f), 50.0f, 0.6f);

    entities["redLight"] = Entity(&registry);
    entities["redLight"].AddComponent<TransformComponent>(glm::vec3(0.0f, 5.0f, 12.0f), glm::quat(1, 0, 0, 0), glm::vec3(0.2f));
    entities["redLight"].AddComponent<RenderComponent>(redLightModel);
    entities["redLight"].AddComponent<PointLightComponent>(glm::vec3(1.0f, 0.0f, 0.0f));

    entities["greenLight"] = Entity(&registry);
    entities["greenLight"].AddComponent<TransformComponent>(glm::vec3(-2.0f, 1.0f, -2.0f), glm::quat(1, 0, 0, 0), glm::vec3(0.2f));
    entities["greenLight"].AddComponent<RenderComponent>(greenLightModel);
    entities["greenLight"].AddComponent<PointLightComponent>(glm::vec3(0.0f, 1.0f, 0.0f));

    entities["flashlight"] = Entity(&registry);
    entities["flashlight"].AddComponent<TransformComponent>(glm::vec3(0.0f, 10.0f, 0.0f), glm::quat(glm::radians(glm::vec3(-45.0f, -90.0f, 0.0f))), glm::vec3(0.05f));
    entities["flashlight"].AddComponent<RenderComponent>(flashLightModel);
    entities["flashlight"].AddComponent<SpotLightComponent>(glm::vec3(1.0f, 1.0f, 0.0f), 15.0f, 20.0f, 100.0f);
}

void PlayState::Update(float dt) {
    // --- PAUSE LOGIC ---
    if (Input::keyWentDown(GLFW_KEY_ESCAPE)) {
        app->PushState(std::make_unique<PauseState>(app));
        return; 
    }

    // --- GAME LOGIC ---
    auto& flagTc = entities["flag"].GetComponent<TransformComponent>();
    transformSystem.Rotate(flagTc, 40 * dt, glm::vec3(0.0f, 0.0f, 1.0f));
   
    auto& sphereTc = entities["sphere"].GetComponent<TransformComponent>();
    transformSystem.Rotate(sphereTc, 50 * dt, glm::vec3(0.0f, 1.0f, 0.0f));
    
    auto& raceTc = entities["race"].GetComponent<TransformComponent>();
    transformSystem.MoveRelative(raceTc, glm::vec3(0.0f, 0.0f, 2.0f) * dt);
    transformSystem.Rotate(raceTc, 30 * dt, glm::vec3(0.0f, 1.0f, 0.0f));
    
    auto& blueLightTc = entities["blueLight"].GetComponent<TransformComponent>();
    transformSystem.MoveRelative(blueLightTc, glm::vec3(0.0f, 0.0f, 2.0f) * dt);
    transformSystem.Rotate(blueLightTc, 30 * dt, glm::vec3(0.0f, 1.0f, 0.0f));
    
    auto& flashLightTc = entities["flashlight"].GetComponent<TransformComponent>();
    transformSystem.Rotate(flashLightTc, 60 * dt, glm::vec3(0.0f, 1.0f, 0.0f));

    // Update Systems
    transformSystem.Update(registry);
    lightSystem.Update(registry, camera->getPosition());

    // --- UPDATE CAMERA ---
    camera->Update(dt);
    camera->processMouseScroll(Input::getScrollDY());
    camera->UpdateUBO(app->GetWindow()->getWidth(), app->GetWindow()->getHeight());
}

void PlayState::Render() {
    renderSystem.Render(registry, app->GetAssets(), camera->getPosition());
}