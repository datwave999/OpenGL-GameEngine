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
    vec2 padding;
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

// --- UBO: LIGHT DATA (Binding 1) ---
#define MAX_POINT_LIGHTS 10

layout(std140, binding = 1) uniform LightData {
    DirectionalLight directionalLight;
    PointLight pointLights[MAX_POINT_LIGHTS];
    int numPointLights;
    vec3 paddingEnd; 
};

// --- FUNCTION PROTOTYPES ---
vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 albedo);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo);

void main() {
    // 1. Setup global math variables
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(cameraPos - FragPos);
    
    // 2. Sample the texture
    vec3 albedo = texture(texture1, TexCoords).rgb;

    // 3. Directional Light calculation
    vec3 result = CalcDirectionalLight(directionalLight, norm, viewDir, albedo);

    // 4. Point Lights calculation
    for (int i = 0; i < numPointLights; i++) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, albedo);
    }

    // 5. Final Output
    FragColour = vec4(result, 1.0);
}

// --------------------------------------------------------
// --- FUNCTION IMPLEMENTATIONS ---
// --------------------------------------------------------

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 albedo) {
    vec3 lightDir = normalize(-light.direction);
    
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
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Diffuse calculation
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Specular calculation (Blinn-Phong)
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), max(matShininess, 0.0001)); 

    // Attenuation (Distance Fading)
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Combine
    vec3 ambient = light.ambient * albedo * attenuation;
    vec3 diffuse = light.diffuse * diff * albedo * attenuation;
    vec3 specular = light.specular * matSpecularIntens * spec * attenuation; 

    return (ambient + diffuse + specular);
}