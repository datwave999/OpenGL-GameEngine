#pragma once

#include "Object.h"
#include "PointLight.h"
#include "LightManager.h"
#include <memory>

class LightObject : public Object
{
public:
	LightObject(std::shared_ptr<Model> model, std::shared_ptr<PointLight> light);

	void Render(Shader* shader) override;

private:
	std::shared_ptr<PointLight> lightData;

};

