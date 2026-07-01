#pragma once

#include "deprecated/Object.h"
#include "deprecated/PositionalLight.h"
#include "deprecated/LightManager.h"
#include <memory>

class LightObject : public Object
{
public:
	LightObject(std::shared_ptr<Model> model, std::shared_ptr<PositionalLight> light, glm::vec3 lightOffset = glm::vec3(0.0f));

	void setLightOffset(glm::vec3 newOffset);

	void Update(float dt) override;
	void Render(Shader* shader) override;

private:
	std::shared_ptr<PositionalLight> lightData;
	glm::vec3 localLightOffset;
};

