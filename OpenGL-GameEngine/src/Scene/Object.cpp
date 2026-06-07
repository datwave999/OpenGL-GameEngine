#include "Object.h"


Object::Object(Model* assignedModel) : model(assignedModel) {}

void Object::Render(Shader* shader) {
	if (!model) return;

	shader->setUniform("model", transform.getModelMatrix());
	
	model->Render(shader);
}