#include "deprecated/LightObject.h"

LightObject::LightObject(std::shared_ptr<Model> model, std::shared_ptr<PositionalLight> light, glm::vec3 lightOffset) : Object(model), lightData(light), localLightOffset(lightOffset)
{
	// To initially match positions & rotations of model and light
	transform.SetPosition(lightData->getPosition());
	transform.SetRotation(glm::quatLookAt(lightData->getDirection(), transform.GetUp()));

	lightData->setPosition(transform.GetPosition() + localLightOffset);
}

void LightObject::Update(float dt) {
	if (transform.hasTransformed()) {
		lightData->setPosition(transform.GetPosition() + localLightOffset);
		lightData->setDirection(transform.GetForward());
	}
}

void LightObject::Render(Shader* shader)
{
	if (!model) return;

	shader->setUniform(Uniform::model, transform.getModelMatrix());

	model->Render(shader);
}
