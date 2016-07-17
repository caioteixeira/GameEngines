#include "ITPEnginePCH.h"

IMPL_COMPONENT(PointLightComponent, Component, MAX_POINT_LIGHTS);

PointLightComponent::PointLightComponent(Actor& owner)
	:Component(owner)
{
	SetEnabled(true);
	SetPosition(owner.GetPosition());
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
	SetPosition(mOwner.GetPosition());
}

void PointLightComponent::SetProperties(const rapidjson::Value& properties)
{
	Super::SetProperties(properties);

	Vector3 diffuse;
	Vector3 specular;
	float innerRadius;
	float outerRadius;
	float specularPower;

	if(GetVectorFromJSON(properties, "diffuseColor", diffuse))
	{
		SetDiffuse(diffuse);
	}

	if(GetVectorFromJSON(properties, "specularColor", specular))
	{
		SetSpecular(specular);
	}

	if(GetFloatFromJSON(properties, "innerRadius", innerRadius))
	{
		SetInnerRadius(innerRadius);
	}

	if(GetFloatFromJSON(properties, "outerRadius", outerRadius))
	{
		SetOuterRadius(outerRadius);
	}

	if(GetFloatFromJSON(properties, "specularPower", specularPower))
	{
		SetSpecularPower(specularPower);
	}
}

