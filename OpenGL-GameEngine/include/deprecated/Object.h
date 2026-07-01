#pragma once

#include "deprecated/Transform.h"
#include "deprecated/Model.h"
#include "Graphics/Shader.h"
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
