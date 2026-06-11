#pragma once
#include "Buffer.h"
#include "LightData.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include <vector>
#include <memory>
#include <string>

class LightManager {
public:
    void Initialize();

    void setDirectionalLight(glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f),
                        glm::vec3 color = glm::vec3(1.0f),
                        float diffuseIntensity = 1.0f,
                        float ambientIntensity = 0.1f);

    std::shared_ptr<PointLight> getPointLight(std::string key, 
                                            glm::vec3 position, glm::vec3 color, float radius = 50.0f,
                                            float diffuseIntensity = 1.0f,
                                            float ambientIntensity = 0.05f,
                                            float specularIntensity = 1.0f);

    void Update();

private:
    // Lights
    std::unique_ptr<DirectionalLight> mainLight;
    std::unordered_map<std::string, std::weak_ptr<PointLight>> pointLights;

    // Light UBO data
    std::unique_ptr<Buffer> lightUBO;
    LightUBO uboData;
};