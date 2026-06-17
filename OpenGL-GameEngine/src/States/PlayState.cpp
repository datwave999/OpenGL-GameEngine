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
#include "LightObject.h"
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

    // 2. Grab AssetManager from Application
    AssetContainer& assets = app->GetAssets();

    // 3. Compile Shaders 
    coreShader = assets.getShader("coreShader", "assets/Shaders/core.vert", "assets/Shaders/core.frag");
    overlayShaderPreload = assets.getShader("overlayShader", "assets/Shaders/overlay.vert", "assets/Shaders/overlay.frag");
    unlitShader = assets.getShader("unlitShader", "assets/Shaders/unlit.vert", "assets/Shaders/unlit.frag");

    // 4. Register Meshes
    auto cubeMesh = assets.getMesh("cube", StandardMeshes::CreateCube(2.0f));
    auto sphereMesh = assets.getMesh("sphere", StandardMeshes::CreateSphere());
    auto planeMesh = assets.getMesh("plane", StandardMeshes::CreatePlane(10.0f));

    // 5. Get Textures 
    auto obamaTex = assets.getTexture("obama", "assets/Textures/obama_sandwich.jpg");
    auto flagTex = assets.getTexture("flag", "assets/Textures/community.png");
    auto greyPrototypeTex = assets.getTexture("prototypeGrey", "assets/Textures/PNG/Light/texture_08.png");
    auto blueTex = assets.getTexture("blue", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    auto redTex = assets.getTexture("red", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    auto greenTex = assets.getTexture("green", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

    // 6. Get Materials
    auto obamaMat = assets.getMaterial("obamaSandwich", obamaTex, nullptr, 0, 1, 64.0f, 1.0f);
    auto flagMat = assets.getMaterial("communityFlag", flagTex, nullptr, 0, 1, 16.0f, 0.6f);
    auto greyMat = assets.getMaterial("greyFloor", greyPrototypeTex, nullptr, 0, 1, 16.0f, 0.6f);
    auto blueMat = assets.getMaterial("blueMat", blueTex, nullptr, 0, 1);
    auto redMat = assets.getMaterial("redMat", redTex, nullptr, 0, 1);
    auto greenMat = assets.getMaterial("greenMat", greenTex, nullptr, 0, 1);

    // 7. Get Models
    auto obamaCubeModel = assets.getModel("obamaCube", cubeMesh, obamaMat);
    auto flagCubeModel = assets.getModel("flagCube", cubeMesh, flagMat);
    auto sandwichSphereModel = assets.getModel("sandwichSphere", sphereMesh, obamaMat);
    auto floorModel = assets.getModel("floor", planeMesh, greyMat);

    auto sedan = assets.getModel("sedan", "assets/Models/sedan/sedan.obj");
    auto race = assets.getModel("race", "assets/Models/race/race.obj");
    auto backpack = assets.getModel("backpack", "assets/Models/backpack/backpack.obj", true);

    auto blueSphere = assets.getModel("blueLight", sphereMesh, blueMat);
    auto redSphere = assets.getModel("redLight", sphereMesh, redMat);
    auto greenSphere = assets.getModel("greenLight", sphereMesh, greenMat);

    auto flashLight = assets.getModel("flashlight", "assets/Models/flashlight/silencer-small.fbx");

    // 8. Create Objects 
    objects.push_back(std::make_unique<Object>(obamaCubeModel));
    objects.push_back(std::make_unique<Object>(flagCubeModel));
    objects.push_back(std::make_unique<Object>(sandwichSphereModel));
    objects.push_back(std::make_unique<Object>(sedan));
    objects.push_back(std::make_unique<Object>(race));
    objects.push_back(std::make_unique<Object>(floorModel));
    objects.push_back(std::make_unique<Object>(backpack));

    objects[1]->transform.SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));
    objects[2]->transform.SetPosition(glm::vec3(0.0f, -1.0f, 3.0f));
    objects[3]->transform.SetPosition(glm::vec3(2.0f, 0.0f, 0.0f));
    objects[4]->transform.SetPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
    objects[5]->transform.SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));
    objects[5]->transform.SetScale(glm::vec3(100.0f, 0.1f, 100.0f));
    objects[6]->transform.SetPosition(glm::vec3(0.0f, 3.0f, 10.0f));

    // 10. Add Lighting to the Scene
    lights.Initialize();
 
    lights.setDirectionalLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.8f, 0.8f, 0.8f), 0.8f); // Whiteish sun

    auto blueLight = lights.addPointLight(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 50.0f, 0.6f); // Blue
    auto redLight = lights.addPointLight(glm::vec3(0.0f, 5.0f, 12.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Red
    auto greenLight = lights.addPointLight(glm::vec3(-2.0f, 1.0f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Green

    auto yellowSpotLight = lights.addSpotLight(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), 15.0f, 20.0f, 100.0f);

    lightObjects.push_back(std::make_unique<LightObject>(blueSphere, blueLight));
    lightObjects.push_back(std::make_unique<LightObject>(redSphere, redLight));
    lightObjects.push_back(std::make_unique<LightObject>(greenSphere, greenLight));
    lightObjects.push_back(std::make_unique<LightObject>(flashLight, yellowSpotLight));

    lightObjects[0]->transform.SetScale(glm::vec3(0.2f));
    lightObjects[1]->transform.SetScale(glm::vec3(0.2f));
    lightObjects[2]->transform.SetScale(glm::vec3(0.2f));
    lightObjects[3]->transform.SetScale(glm::vec3(0.05f));
}

void PlayState::Update(Application* app, float dt) {
    // --- PAUSE LOGIC ---
    if (Input::keyWentDown(GLFW_KEY_ESCAPE)) {
        app->PushState(std::make_unique<PauseState>());
        return; 
    }

    // --- GAME LOGIC ---
    objects[1]->transform.Rotate(40 * dt, glm::vec3(0.0f, 0.0f, 1.0f));
    objects[2]->transform.Rotate(50 * dt, glm::vec3(0.0f, 1.0f, 0.0f));

    objects[4]->transform.MoveRelative(glm::vec3(0.0f, 0.0f, 2.0f) * dt);
    objects[4]->transform.Rotate(30 * dt, glm::vec3(0.0f, 1.0f, 0.0f));

    lightObjects[0]->transform.MoveRelative(glm::vec3(0.0f, 0.0f, 2.0f) * dt);
    lightObjects[0]->transform.Rotate(30 * dt, glm::vec3(0.0f, 1.0f, 0.0f));
    lightObjects[3]->transform.Rotate(60 * dt, glm::vec3(0.0f, 1.0f, 0.0f));

    // --- UPDATE LIGHTING ---
    for (const auto& obj : objects) {
        obj->Update(dt);
    }

    for (const auto& lightObj : lightObjects) {
        lightObj->Update(dt);
    }

    lights.UpdateData(camera->getPosition());

    // --- UPDATE CAMERA ---
    camera->Update(dt);
    camera->processMouseScroll(Input::getScrollDY());
    camera->UpdateUBO(app->GetWindow()->getWidth(), app->GetWindow()->getHeight());
}

void PlayState::Render(Application* app) {
    // Core shader for Objects
    coreShader->enableShader();

        coreShader->setUniform(Uniform::cameraPos, camera->getPosition());

        for (const auto& obj : objects) {
            obj->Render(coreShader.get());
        }

    coreShader->disableShader();

    // Unlit shader for Light Objects
    unlitShader->enableShader();

    for (const auto& lightObj : lightObjects) {
        lightObj->Render(unlitShader.get());
    }

    unlitShader->disableShader();
}
