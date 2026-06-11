#pragma once

#include "Transform.h"
#include "Model.h"
#include "Shader.h"
#include <memory>

class Object
{
public:
	Transform transform;

	Object(const std::shared_ptr<Model>& assignedModel);

	virtual void Update(float dt);
	virtual void Render(Shader* shader);

protected:
	std::shared_ptr<Model> model;
};
