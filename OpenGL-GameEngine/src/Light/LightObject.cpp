#include "LightObject.h"

LightObject::LightObject(std::shared_ptr<Model> model, std::shared_ptr<PointLight> light) : Object(model), lightData(light)
{
	// To initially match positions of model and light
	transform.SetPosition(lightData->getPosition());
}

void LightObject::Update(float dt) {
	if (transform.hasTransformed()) lightData->setPosition(transform.GetPosition());
}

void LightObject::Render(Shader* shader)
{
	if (!model) return;

	shader->setUniform(Uniform::model, transform.getModelMatrix());

	model->Render(shader);
}
