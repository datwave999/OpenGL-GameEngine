#include "Object.h"


Object::Object(const std::shared_ptr<Model>& assignedModel) : model(assignedModel) {}

void Object::Render(Shader* shader) {
	if (!model) return;

	shader->setUniform(Uniform::model, transform.getModelMatrix());
	shader->setUniform(Uniform::normalMatrix, transform.getNormalMatrix());

	model->Render(shader);
}