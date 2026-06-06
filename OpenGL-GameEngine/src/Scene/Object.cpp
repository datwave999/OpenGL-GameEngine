#include "Object.h"


Object::Object(Mesh* assignedMesh, Material* assignedMaterial) : mesh(assignedMesh), material(assignedMaterial) {}

void Object::Render(Shader* shader) {
	if (!mesh || !material) return;

	shader->setUniform("model", transform.getModelMatrix());
	
	material->Use(shader);

	mesh->RenderMesh();
}