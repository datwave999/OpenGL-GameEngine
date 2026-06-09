#include "Object.h"


Object::Object(const std::shared_ptr<Model>& assignedModel) : model(assignedModel) {}

void Object::Render(Shader* shader) {
	if (!model) return;

	glm::mat4 modelMatrix = transform.getModelMatrix();
	shader->setUniform("model", modelMatrix);
	
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
	shader->setUniform("normalMatrix", normalMatrix);

	model->Render(shader);
}