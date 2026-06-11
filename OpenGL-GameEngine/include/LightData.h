#pragma once
#include <glm/glm.hpp>

// 1. Directional Light Struct
struct DirectionalLightData {
    glm::vec3 direction;
    float padding1;

    glm::vec3 ambient;
    float padding2;

    glm::vec3 diffuse;
    float padding3;

    glm::vec3 specular;
    float padding4;
};

// 2. Point Light Struct
struct PointLightData {
    glm::vec3 position;
    float padding1;

    glm::vec3 ambient;
    float constant;

    glm::vec3 diffuse;
    float linear;

    glm::vec3 specular;
    float quadratic;
};

// 3. The Master UBO Block
#define MAX_POINT_LIGHTS 10

struct LightUBO {
    DirectionalLightData directionalLight;
    PointLightData pointLights[MAX_POINT_LIGHTS];
    int numPointLights;
    float pad1, pad2, pad3;
};