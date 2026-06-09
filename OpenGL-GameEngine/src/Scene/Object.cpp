#include "Object.h"


Object::Object(const std::shared_ptr<Model>& assignedModel) : model(assignedModel) {}

void Object::Render(Shader* shader) {
	if (!model) return;

	shader->setUniform("model", transform.getModelMatrix());
	shader->setUniform("normalMatrix", transform.getNormalMatrix());

	model->Render(shader);
}