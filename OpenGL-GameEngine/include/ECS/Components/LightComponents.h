#pragma once
#include <glm/glm.hpp>

// 1. Directional Light Component
struct DirectionalLightComponent {
    glm::vec3 direction;
    float padding1;

    glm::vec3 ambient;
    float padding2;

    glm::vec3 diffuse;
    float padding3;

    glm::vec3 specular;
    float padding4;
};

// --- 2. ECS Game Data Components ---

struct PointLightComponent {
    glm::vec3 color{ 1.0f };
    float radius{ 50.0f };
    float diffuseIntensity{ 1.0f };
    float ambientIntensity{ 0.05f };
    float specularIntensity{ 1.0f };
    // Cached Position
    glm::vec3 position{ 0.0f };
};

struct SpotLightComponent {
    glm::vec3 color{ 1.0f };
    float innerCutOffAngle{ 12.5f };
    float outerCutOffAngle{ 17.5f };
    float radius{ 50.0f };
    float diffuseIntensity{ 1.0f };
    float ambientIntensity{ 0.05f };
    float specularIntensity{ 1.0f };
    // Cached position & direction
    glm::vec3 position{ 0.0f };
    glm::vec3 direction{ 0.0f, -1.0f, 0.0f };
};

// --- 3. GPU UBO Structs (Strict std140 Padding) ---

struct PointLightUBOData {
    glm::vec3 position;
    float padding1;
    glm::vec3 ambient;
    float constant;
    glm::vec3 diffuse;
    float linear;
    glm::vec3 specular;
    float quadratic;
};

struct SpotLightUBOData {
    glm::vec3 position;
    float cutOff;
    glm::vec3 direction;
    float outerCutOff;
    glm::vec3 ambient;
    float constant;
    glm::vec3 diffuse;
    float linear;
    glm::vec3 specular;
    float quadratic;
};

// --- 4. The Master UBO Block ---
#define MAX_POINT_LIGHTS 16
#define MAX_SPOT_LIGHTS 8

struct LightUBO {
    DirectionalLightComponent directionalLight; // (Keep as is, or split similarly)
    PointLightUBOData pointLights[MAX_POINT_LIGHTS];
    SpotLightUBOData spotLights[MAX_SPOT_LIGHTS];
    int numPointLights;
    int numSpotLights;
    float pad1, pad2;
};


// 5. Offset Light w.r.t Model
struct LightOffsetComponent {
    glm::vec3 localOffset{ 0.0f };
    glm::vec3 directionOffset{ 0.0f, 0.0f, -1.0f };
};


