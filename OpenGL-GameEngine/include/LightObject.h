#pragma once

#include "Object.h"
#include "PositionalLight.h"
#include "LightManager.h"
#include <memory>

class LightObject : public Object
{
public:
	LightObject(std::shared_ptr<Model> model, std::shared_ptr<PositionalLight> light);

	void Update(float dt) override;
	void Render(Shader* shader) override;

private:
	std::shared_ptr<PositionalLight> lightData;

};

