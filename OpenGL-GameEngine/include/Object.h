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

	void Render(Shader* shader);

private:
	std::shared_ptr<Model> model;
};
