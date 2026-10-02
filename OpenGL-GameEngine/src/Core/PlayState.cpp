#include "Core/PlayState.h"
#include "Core/PauseState.h" // To push onto stack when we pause
#include "Core/Application.h"
#include "Core/Window.h"
#include "Core/Camera.h"
#include "Graphics/Shader.h"
#include "Core/Input.h"
#include "Resources/StandardMeshes.h"
#include <cmath>
#include <iostream>

PlayState::PlayState(Application* app) : State(app) {}

PlayState::~PlayState() {}

void PlayState::Initialize() {
    // 1. Setup Input & Camera
    glfwSetInputMode(app->GetWindow()->getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    camera = std::make_unique<Camera>(glm::vec3(0.0f, 1.8f, 11.0f));

    // 2. Grab AssetManager from Application
    AssetManager& assets = app->GetAssets();

    // 3. Compile Shaders & pre-warm shaders
    coreShaderHandle = assets.LoadShader("core", "assets/Shaders/core.vert", "assets/Shaders/core.frag");
    uint32_t unlitShaderHandle = assets.LoadShader("unlit", "assets/Shaders/unlit.vert", "assets/Shaders/unlit.frag");
    assets.LoadShader("overlay", "assets/Shaders/overlay.vert", "assets/Shaders/overlay.frag");

    // 4. Get Meshes
    auto cubeMesh = assets.CreateMesh("cube", StandardMeshes::CreateCube(1.0f));
    auto sphereMesh = assets.CreateMesh("sphere", StandardMeshes::CreateSphere());
    
    // Colors for Textures and Lighting
    const glm::vec3 blueColour(0.025f, 0.08f, 1.0f);
    const glm::vec3 sedanLightColour(1.0f, 0.04f, 0.03f);
    const glm::vec3 raceLightColour(0.03f, 1.0f, 0.2f);
    const glm::vec3 displayLightColour(1.0f, 0.05f, 0.65f);

    // 5. Get Textures 
    auto obamaTex = assets.LoadTexture("obama", "assets/Textures/obama_sandwich.jpg");
    auto pedestalTex = assets.LoadTexture("woodCrate", "assets/Textures/wood_crate.jpg");
    auto whiteTex = assets.CreateTexture("showcaseWhite", glm::vec4(0.85f, 0.85f, 0.85f, 1.0f));
    auto blueTex = assets.CreateTexture("showcaseBlue", glm::vec4(blueColour, 1.0f));
    auto redTex = assets.CreateTexture("sedanLightColour", glm::vec4(sedanLightColour, 1.0f));
    auto greenTex = assets.CreateTexture("raceLightColour", glm::vec4(raceLightColour, 1.0f));
    auto displayLightTex = assets.CreateTexture("displayLightColour", glm::vec4(displayLightColour, 1.0f));

    // 6. Get Materials
    auto obamaMat = assets.CreateMaterial("obamaSandwich", obamaTex, UINT32_MAX, 64.0f, 1.0f, coreShaderHandle);
    auto pedestalMat = assets.CreateMaterial("displayBase", pedestalTex, UINT32_MAX, 16.0f, 0.15f, coreShaderHandle);
    auto sphereMat = assets.CreateMaterial("glossySphere", whiteTex, UINT32_MAX, 128.0f, 0.8f, coreShaderHandle);

    auto blueMat = assets.CreateMaterial("blueMat", blueTex, UINT32_MAX, 1.0f, 1.0f, unlitShaderHandle);
    auto redMat = assets.CreateMaterial("redMat", redTex, UINT32_MAX, 1.0f, 1.0f, unlitShaderHandle);
    auto greenMat = assets.CreateMaterial("greenMat", greenTex, UINT32_MAX, 1.0f, 1.0f, unlitShaderHandle);
    auto displayLightMat = assets.CreateMaterial("displayLightMat", displayLightTex, UINT32_MAX, 1.0f, 1.0f, unlitShaderHandle);

    // 7. Models
    auto obamaModel = assets.CreateModel("obamaCube", cubeMesh, obamaMat);
    auto sphereModel = assets.CreateModel("glossySphere", sphereMesh, sphereMat);
    auto baseModel = assets.CreateModel("displayBase", cubeMesh, pedestalMat);

    auto sedanModel = assets.LoadModel("sedan", "assets/Models/sedan/sedan.obj", coreShaderHandle);
    auto raceModel = assets.LoadModel("race", "assets/Models/race/race.obj", coreShaderHandle);
    auto backpackModel = assets.LoadModel("backpack", "assets/Models/backpack/backpack.obj", coreShaderHandle, true);
    auto sponzaModel = assets.LoadModel("sponza", "assets/Models/sponza/sponza.obj", coreShaderHandle);
    auto blueLightModel = assets.CreateModel("blueLight", sphereMesh, blueMat);
    auto redLightModel = assets.CreateModel("redLight", sphereMesh, redMat);
    auto greenLightModel = assets.CreateModel("greenLight", sphereMesh, greenMat);
    auto displayLightModel = assets.CreateModel("displayLight", sphereMesh, displayLightMat);
    auto flashLightModel = assets.LoadModel("flashlight", "assets/Models/flashlight/silencer-small.fbx", unlitShaderHandle);

    // Set Directional Light, Dim Yellowish Light
    lightSystem.SetDirectionalLight(glm::normalize(glm::vec3(-0.2f, -1.0f, -0.3f)), glm::vec3(0.35f, 0.34f, 0.30f), 0.40f, 0.15f);

    // 8. Create Entities

        // sponza model
    entities["sponza"] = Entity(&registry);
    entities["sponza"].AddComponent<TransformComponent>(glm::vec3(35.6229f, 2.5062f, -65.00175f) * 0.025f, glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(0.025f));
    entities["sponza"].AddComponent<RenderComponent>(sponzaModel);

        // Display Bases (wooden crates)
    const float displayPositions[] = { -3.5f, 0.0f, 3.5f };
    for (int i = 0; i < 3; ++i) {
        Entity base(&registry);
        base.AddComponent<TransformComponent>(glm::vec3(displayPositions[i], 0.3f, 0.0f), glm::quat(1, 0, 0, 0), glm::vec3(1.8f, 0.6f, 1.8f));
        base.AddComponent<RenderComponent>(baseModel);
    }
        
        // Displayed Entities
    entities["obamaCube"] = Entity(&registry);
    entities["obamaCube"].AddComponent<TransformComponent>(glm::vec3(0.0f, 1.5f, 0.0f), glm::quat(glm::radians(glm::vec3(15.0f, 0.0f, 10.0f))), glm::vec3(1.0f));
    entities["obamaCube"].AddComponent<RenderComponent>(obamaModel);

    entities["sphere"] = Entity(&registry);
    entities["sphere"].AddComponent<TransformComponent>(glm::vec3(-3.5f, 1.1f, 0.0f));
    entities["sphere"].AddComponent<RenderComponent>(sphereModel);

    entities["sedan"] = Entity(&registry);
    entities["sedan"].AddComponent<TransformComponent>(glm::vec3(-3.5f, 0.0f, -5.0f), glm::angleAxis(glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(1.0f));
    entities["sedan"].AddComponent<RenderComponent>(sedanModel);

    entities["race"] = Entity(&registry);
    entities["race"].AddComponent<TransformComponent>(glm::vec3(3.5f, 0.0f, -5.0f), glm::angleAxis(glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(1.0f));
    entities["race"].AddComponent<RenderComponent>(raceModel);

    entities["backpack"] = Entity(&registry);
    entities["backpack"].AddComponent<TransformComponent>(glm::vec3(3.5f, 1.296407f, 0.377251f), glm::quat(1, 0, 0, 0), glm::vec3(0.4f));
    entities["backpack"].AddComponent<RenderComponent>(backpackModel);

        // Point Lights
    entities["blueLight"] = Entity(&registry);
    entities["blueLight"].AddComponent<TransformComponent>(glm::vec3(2.0f, 2.0f, 0.0f), glm::quat(1, 0, 0, 0), glm::vec3(0.15f));
    entities["blueLight"].AddComponent<RenderComponent>(blueLightModel);
    entities["blueLight"].AddComponent<PointLightComponent>(blueColour, 16.0f, 3.0f, 0.0f, 1.2f);

    entities["bigLight"] = Entity(&registry);
    entities["bigLight"].AddComponent<TransformComponent>(glm::vec3(0.0f, 2.0f, 15.0f), glm::quat(1, 0, 0, 0), glm::vec3(0.35f));
    entities["bigLight"].AddComponent<RenderComponent>(blueLightModel);
    entities["bigLight"].AddComponent<PointLightComponent>(blueColour, 100.0f, 1.0f, 0.0f, 0.8f);

    entities["sedanLight"] = Entity(&registry);
    entities["sedanLight"].AddComponent<TransformComponent>(glm::vec3(-2.78f, 0.8f, -3.03f), glm::quat(1, 0, 0, 0), glm::vec3(0.15f));
    entities["sedanLight"].AddComponent<RenderComponent>(redLightModel);
    entities["sedanLight"].AddComponent<PointLightComponent>(sedanLightColour, 12.0f, 2.2f, 0.0f, 1.0f);

    entities["raceLight"] = Entity(&registry);
    entities["raceLight"].AddComponent<TransformComponent>(glm::vec3(2.78f, 0.8f, -3.03f), glm::quat(1, 0, 0, 0), glm::vec3(0.15f));
    entities["raceLight"].AddComponent<RenderComponent>(greenLightModel);
    entities["raceLight"].AddComponent<PointLightComponent>(raceLightColour, 12.0f, 2.2f, 0.0f, 1.0f);

    entities["displayLight"] = Entity(&registry);
    entities["displayLight"].AddComponent<TransformComponent>(glm::vec3(0.0f, 1.5f, 1.2f), glm::quat(1, 0, 0, 0), glm::vec3(0.15f));
    entities["displayLight"].AddComponent<RenderComponent>(displayLightModel);
    entities["displayLight"].AddComponent<PointLightComponent>(displayLightColour, 9.0f, 2.0f, 0.0f, 1.0f);

        // Rotating Yellow SpotLight
    entities["flashlight"] = Entity(&registry);
    entities["flashlight"].AddComponent<TransformComponent>(glm::vec3(0.0f, 6.0f, -2.0f), glm::quat(glm::radians(glm::vec3(-60.0f, -90.0f, 0.0f))), glm::vec3(0.05f));
    entities["flashlight"].AddComponent<RenderComponent>(flashLightModel);
    entities["flashlight"].AddComponent<SpotLightComponent>(glm::vec3(1.0f, 0.82f, 0.08f), 15.0f, 22.5f, 45.0f, 2.8f, 0.0f, 1.2f);

    sceneTime = 0.0f;
}

void PlayState::Update(float dt) {
    // --- PAUSE LOGIC ---
    if (Input::keyWentDown(GLFW_KEY_ESCAPE)) {
        app->PushState(std::make_unique<PauseState>(app));
        return; 
    }

    // Dynamic Model Loading
    if (Input::keyWentDown(GLFW_KEY_L)) {
        LoadModelFromConsole(coreShaderHandle);
    }

    // Camera update
    camera->Update(dt);

    // --- GAME LOGIC ---
    sceneTime = std::fmod(sceneTime + dt, 2.0f * 3.14159265f / 0.5f);

    auto& obamaTc = entities["obamaCube"].GetComponent<TransformComponent>();
    transformSystem.Rotate(obamaTc, 35.0f * dt, glm::vec3(0.0f, 1.0f, 0.0f));
    
    auto& blueLightTc = entities["blueLight"].GetComponent<TransformComponent>();
    transformSystem.SetPosition(blueLightTc, glm::vec3(2.0f * std::cos(sceneTime * 0.5f), 2.0f, 2.0f * std::sin(sceneTime * 0.5f)));

    auto& displayLightTc = entities["displayLight"].GetComponent<TransformComponent>();
    transformSystem.SetPosition(displayLightTc, glm::vec3(3.5f * std::sin(sceneTime * 0.5f), 1.5f, 1.2f));

    auto& flashLightTc = entities["flashlight"].GetComponent<TransformComponent>();
    transformSystem.Rotate(flashLightTc, 60.0f * dt, glm::vec3(0.0f, 1.0f, 0.0f));

    // Update Systems
    transformSystem.Update(registry);
    lightSystem.Update(registry, camera->getPosition());

    // Upload updated UBO data to camera
    camera->UpdateUBO(app->GetWindow()->getWidth(), app->GetWindow()->getHeight());
}

void PlayState::Render() {
    renderSystem.Render(registry, app->GetAssets(), camera->getPosition());
}

void PlayState::LoadModelFromConsole(uint32_t shaderHandle)
{
    // Input key, location and flipUVs to spawn new model
    std::string key;
    std::string path;
    std::string flipUVs;

    std::cout << "\nCache key: " << std::flush;
    if (!std::getline(std::cin, key)) return;

    std::cout << "Model path: " << std::flush;
    if (!std::getline(std::cin, path)) return;

    std::cout << "Flip UVs (0 or 1): " << std::flush;
    if (!std::getline(std::cin, flipUVs)) return;

    if (key.empty() || path.empty() || (flipUVs != "0" && flipUVs != "1")) {
        std::cout << "Enter a nonempty key/path and 0 or 1 in FlipUVs!\n";
        return;
    }

    // Load The Model Dynamically
    const uint32_t modelHandle = app->GetAssets().LoadModel(key, path, coreShaderHandle, flipUVs == "1");

    if (modelHandle == UINT32_MAX) return;

    // Spawn newly loaded model infront of the camera
    const glm::vec3 position = camera->getPosition() + camera->getFront() * 3.0f;

    Entity object(&registry);
    object.AddComponent<TransformComponent>(position);
    object.AddComponent<RenderComponent>(modelHandle);

    std::cout << "Spawned: " << key << '\n';
}
