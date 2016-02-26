#include "ITPEnginePCH.h"

IMPL_COMPONENT(PointLightComponent, Component, MAX_POINT_LIGHTS);

PointLightComponent::PointLightComponent(Actor& owner)
	:Component(owner)
{
	// TODO: Lab 3
}

void PointLightComponent::Register()
{
	Super::Register();
	mOwner.GetGame().GetRenderer().AddPointLight(ThisPtr());
}

void PointLightComponent::Unregister()
{
	Super::Unregister();
	mOwner.GetGame().GetRenderer().RemovePointLight(ThisPtr());
}

void PointLightComponent::OnUpdatedTransform()
{
	// TODO: Lab 3
}

void PointLightComponent::SetProperties(const rapidjson::Value& properties)
{
	Super::SetProperties(properties);

	// TODO: Lab 3
}

