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

    // 5. Get Textures 
    auto obamaTex = assets.LoadTexture("obama", "assets/Textures/obama_sandwich.jpg");
    auto greyPrototypeTex = assets.LoadTexture("prototypeGrey", "assets/Textures/PNG/Light/texture_08.png");
    auto whiteTex = assets.CreateTexture("showcaseWhite", glm::vec4(0.85f, 0.85f, 0.85f, 1.0f));
    auto blueTex = assets.CreateTexture("blue", glm::vec4(0.2f, 0.35f, 1.0f, 1.0f));
    auto redTex = assets.CreateTexture("red", glm::vec4(1.0f, 0.58f, 0.28f, 1.0f));
    auto greenTex = assets.CreateTexture("green", glm::vec4(0.25f, 0.8f, 0.6f, 1.0f));

    // 6. Get Materials
    auto obamaMat = assets.CreateMaterial("obamaSandwich", obamaTex, UINT32_MAX, 64.0f, 1.0f, coreShaderHandle);
    auto greyMat = assets.CreateMaterial("displayBase", greyPrototypeTex, UINT32_MAX, 16.0f, 0.15f, coreShaderHandle);
    auto sphereMat = assets.CreateMaterial("glossySphere", whiteTex, UINT32_MAX, 128.0f, 0.8f, coreShaderHandle);

    auto blueMat = assets.CreateMaterial("blueMat", blueTex, UINT32_MAX, 1.0f, 1.0f, unlitShaderHandle);
    auto redMat = assets.CreateMaterial("redMat", redTex, UINT32_MAX, 1.0f, 1.0f, unlitShaderHandle);
    auto greenMat = assets.CreateMaterial("greenMat", greenTex, UINT32_MAX, 1.0f, 1.0f, unlitShaderHandle);

    // 7. Models
    auto obamaModel = assets.CreateModel("obamaCube", cubeMesh, obamaMat);
    auto sphereModel = assets.CreateModel("glossySphere", sphereMesh, sphereMat);
    auto baseModel = assets.CreateModel("displayBase", cubeMesh, greyMat);

    auto sedanModel = assets.LoadModel("sedan", "assets/Models/sedan/sedan.obj", coreShaderHandle);
    auto raceModel = assets.LoadModel("race", "assets/Models/race/race.obj", coreShaderHandle);
    auto backpackModel = assets.LoadModel("backpack", "assets/Models/backpack/backpack.obj", coreShaderHandle, true);
    auto sponzaModel = assets.LoadModel("sponza", "assets/Models/sponza/sponza.obj", coreShaderHandle);
    auto blueLightModel = assets.CreateModel("blueLight", sphereMesh, blueMat);
    auto redLightModel = assets.CreateModel("redLight", sphereMesh, redMat);
    auto greenLightModel = assets.CreateModel("greenLight", sphereMesh, greenMat);

    // The Sun
    lightSystem.SetDirectionalLight(glm::normalize(glm::vec3(-0.2f, -1.0f, -0.3f)),
        glm::vec3(0.85f, 0.88f, 1.0f), 0.5f, 0.18f);

    // 8. Create Entities
    if (sponzaModel != UINT32_MAX) {
        // Preserve the chosen scale and centre the ground floor at the origin.
        constexpr float sponzaScale = 0.025f;
        entities["sponza"] = Entity(&registry);
        entities["sponza"].AddComponent<TransformComponent>(
            glm::vec3(35.6229f, 2.5062f, -65.00175f) * sponzaScale,
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
            glm::vec3(sponzaScale));
        entities["sponza"].AddComponent<RenderComponent>(sponzaModel);
    }

    // Three display bases share the same mesh and material.
    const float displayPositions[] = { -3.5f, 0.0f, 3.5f };
    for (int i = 0; i < 3; ++i) {
        Entity base(&registry);
        base.AddComponent<TransformComponent>(glm::vec3(displayPositions[i], 0.3f, 0.0f),
            glm::quat(1, 0, 0, 0), glm::vec3(1.8f, 0.6f, 1.8f));
        base.AddComponent<RenderComponent>(baseModel);
    }

    entities["obamaCube"] = Entity(&registry);
    entities["obamaCube"].AddComponent<TransformComponent>(glm::vec3(0.0f, 1.5f, 0.0f),
        glm::quat(glm::radians(glm::vec3(15.0f, 0.0f, 10.0f))), glm::vec3(1.0f));
    entities["obamaCube"].AddComponent<RenderComponent>(obamaModel);

    entities["sphere"] = Entity(&registry);
    entities["sphere"].AddComponent<TransformComponent>(glm::vec3(-3.5f, 1.1f, 0.0f));
    entities["sphere"].AddComponent<RenderComponent>(sphereModel);

    entities["sedan"] = Entity(&registry);
    entities["sedan"].AddComponent<TransformComponent>(glm::vec3(-3.5f, 0.0f, -5.0f),
        glm::angleAxis(glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(1.0f));
    entities["sedan"].AddComponent<RenderComponent>(sedanModel);

    entities["race"] = Entity(&registry);
    entities["race"].AddComponent<TransformComponent>(glm::vec3(3.5f, 0.0f, -5.0f),
        glm::angleAxis(glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(1.0f));
    entities["race"].AddComponent<RenderComponent>(raceModel);

    entities["backpack"] = Entity(&registry);
    // Account for the backpack's off-centre origin so its bottom rests on the base.
    entities["backpack"].AddComponent<TransformComponent>(glm::vec3(3.5f, 1.296407f, 0.377251f),
        glm::quat(1, 0, 0, 0), glm::vec3(0.4f));
    entities["backpack"].AddComponent<RenderComponent>(backpackModel);

    entities["blueLight"] = Entity(&registry);
    entities["blueLight"].AddComponent<TransformComponent>(glm::vec3(2.0f, 2.5f, 0.0f), glm::quat(1, 0, 0, 0), glm::vec3(0.15f));
    entities["blueLight"].AddComponent<RenderComponent>(blueLightModel);
    entities["blueLight"].AddComponent<PointLightComponent>(glm::vec3(0.2f, 0.35f, 1.0f), 18.0f, 0.8f, 0.0f, 0.8f);

    entities["redLight"] = Entity(&registry);
    entities["redLight"].AddComponent<TransformComponent>(glm::vec3(-5.0f, 3.0f, -4.0f), glm::quat(1, 0, 0, 0), glm::vec3(0.15f));
    entities["redLight"].AddComponent<RenderComponent>(redLightModel);
    entities["redLight"].AddComponent<PointLightComponent>(glm::vec3(1.0f, 0.58f, 0.28f), 30.0f, 1.0f, 0.0f, 0.7f);

    entities["greenLight"] = Entity(&registry);
    entities["greenLight"].AddComponent<TransformComponent>(glm::vec3(5.0f, 3.0f, -4.0f), glm::quat(1, 0, 0, 0), glm::vec3(0.15f));
    entities["greenLight"].AddComponent<RenderComponent>(greenLightModel);
    entities["greenLight"].AddComponent<PointLightComponent>(glm::vec3(0.25f, 0.8f, 0.6f), 25.0f, 0.7f, 0.0f, 0.7f);

    // A downward spotlight highlights the displays without a floating model.
    entities["displaySpot"] = Entity(&registry);
    entities["displaySpot"].AddComponent<TransformComponent>(glm::vec3(0.0f, 6.0f, 0.0f),
        glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    entities["displaySpot"].AddComponent<SpotLightComponent>(glm::vec3(1.0f, 0.92f, 0.8f),
        25.0f, 38.0f, 30.0f, 1.2f, 0.0f, 1.0f);

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
    auto& obamaTc = entities["obamaCube"].GetComponent<TransformComponent>();
    transformSystem.Rotate(obamaTc, 35.0f * dt, glm::vec3(0.0f, 1.0f, 0.0f));
    
    sceneTime = std::fmod(sceneTime + dt, 2.0f * 3.14159265f / 0.5f);
    auto& blueLightTc = entities["blueLight"].GetComponent<TransformComponent>();
    transformSystem.SetPosition(blueLightTc,
        glm::vec3(2.0f * std::cos(sceneTime * 0.5f), 2.5f, 2.0f * std::sin(sceneTime * 0.5f)));

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
