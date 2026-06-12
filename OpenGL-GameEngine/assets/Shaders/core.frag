#version 460 core

out vec4 FragColour;

// --- INPUTS FROM VERTEX SHADER ---
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

// --- STANDARD UNIFORMS ---
uniform sampler2D texture1;
uniform vec3 cameraPos; 

// --- UBO: MATERIAL DATA (Binding 2) ---
layout(std140, binding = 2) uniform MaterialData {
    float matShininess;
    float matSpecularIntens;
    float matPad1, matPad2;
};

// --- LIGHTING STRUCTURES ---
struct DirectionalLight {
    vec3 direction;
    float padding1;
    vec3 ambient;
    float padding2;
    vec3 diffuse;
    float padding3;
    vec3 specular;
    float padding4;
};

struct PointLight {
    vec3 position;
    float padding1;
    vec3 ambient;
    float constant;
    vec3 diffuse;
    float linear;
    vec3 specular;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    float cutOff;
    vec3 direction;
    float outerCutOff;
    vec3 ambient;
    float constant;
    vec3 diffuse;
    float linear;
    vec3 specular;
    float quadratic;
};

// --- UBO: LIGHT DATA (Binding 1) ---
#define MAX_POINT_LIGHTS 16
#define MAX_SPOT_LIGHTS 8

layout(std140, binding = 1) uniform LightData {
    DirectionalLight directionalLight;
    PointLight pointLights[MAX_POINT_LIGHTS];
    SpotLight spotLights[MAX_SPOT_LIGHTS];
    int numPointLights;
    int numSpotLights;
    float pad1, pad2;
};

// --- FUNCTION PROTOTYPES ---
vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 albedo);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo);

void main() {
    // 1. Setup global math variables
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(cameraPos - FragPos);
    
    // 2. Sample the texture
    vec4 texColour = texture(texture1, TexCoords);
    vec3 albedo = texColour.rgb;

    // 3. Directional Light calculation
    vec3 result = CalcDirectionalLight(directionalLight, norm, viewDir, albedo);

    // 4. Point Lights calculation
    for (int i = 0; i < numPointLights; i++) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, albedo);
    }

    // 5. Spot Lights calculation
    for (int i = 0; i < numSpotLights; i++) {
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir, albedo);
    }

    // 6. Final Output
    FragColour = vec4(result, texColour.a);
}

// --------------------------------------------------------
// --- FUNCTION IMPLEMENTATIONS ---
// --------------------------------------------------------

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 albedo) {
    vec3 lightDir = (-light.direction);
    
    // Diffuse calculation
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Specular calculation (Blinn-Phong)
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), max(matShininess, 0.0001)); 

    // Combine
    vec3 ambient = light.ambient * albedo;
    vec3 diffuse = light.diffuse * diff * albedo;
    vec3 specular = light.specular * matSpecularIntens * spec; 

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo) {
    vec3 lightVector = light.position - fragPos;
    float distance = length(lightVector);
    vec3 lightDir = lightVector / distance;
    
    // Diffuse calculation
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Specular calculation (Blinn-Phong)
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), max(matShininess, 0.0001)); 

    // Attenuation (Distance Fading)
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Combine
    vec3 ambient = light.ambient * albedo * attenuation;
    vec3 diffuse = light.diffuse * diff * albedo * attenuation;
    vec3 specular = light.specular * matSpecularIntens * spec * attenuation; 

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo) {
    vec3 lightVector = light.position - fragPos;
    float distance = length(lightVector);
    vec3 lightDir = lightVector / distance;

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), max(matShininess, 0.0001)); 

    // Attenuation
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Spotlight Cone Intensity (Soft Edges)
    float theta = dot(lightDir, (-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // Combine results
    vec3 ambient = light.ambient * albedo * attenuation;
    vec3 diffuse = light.diffuse * diff * albedo * attenuation * intensity;
    vec3 specular = light.specular * matSpecularIntens * spec * attenuation * intensity;

    return (ambient + diffuse + specular);
}