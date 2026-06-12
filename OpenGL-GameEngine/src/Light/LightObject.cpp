#include "LightObject.h"

LightObject::LightObject(std::shared_ptr<Model> model, std::shared_ptr<PositionalLight> light) : Object(model), lightData(light)
{
	// To initially match positions & rotations of model and light
	transform.SetPosition(lightData->getPosition());
	transform.SetRotation(glm::quatLookAt(lightData->getDirection(), transform.GetUp()));
}

void LightObject::Update(float dt) {
	if (transform.hasTransformed()) {
		lightData->setPosition(transform.GetPosition());
		lightData->setDirection(transform.GetForward());
	}
}

void LightObject::Render(Shader* shader)
{
	if (!model) return;

	shader->setUniform(Uniform::model, transform.getModelMatrix());

	model->Render(shader);
}
