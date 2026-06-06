#pragma once

#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"

class Object
{
public:
	Transform transform;

	Object(Mesh* assignedMesh, Material* assignedMaterial);

	void Render(Shader* shader);

private:
	Mesh* mesh;
	Material* material;
};

