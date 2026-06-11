#include "LightObject.h"

LightObject::LightObject(std::shared_ptr<Model> model, std::shared_ptr<PointLight> light) : Object(model)
{
	lightData = light;
	transform.SetPosition(lightData->getPosition());
}

void LightObject::Render(Shader* shader)
{
	lightData->setPosition(transform.GetPosition());

	if (!model) return;

	shader->setUniform(Uniform::model, transform.getModelMatrix());

	model->Render(shader);
}
