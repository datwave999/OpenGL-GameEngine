#pragma once

#include "Transform.h"
#include "Model.h"
#include "Shader.h"

class Object
{
public:
	Transform transform;

	Object(Model* assignedModel);

	void Render(Shader* shader);

private:
	Model* model;
};

