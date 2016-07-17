#pragma once
#include "Component.h"
#include "PointLightData.h"

class PointLightComponent : public Component
{
	DECL_COMPONENT(PointLightComponent, Component);
public:
	PointLightComponent(Actor& owner);

	void Register() override;
	void Unregister() override;

	void OnUpdatedTransform() override;

	const PointLightData& GetData() const { return mData; }

	void SetDiffuse(const Vector3& diffuse){ mData.mDiffuse = diffuse; }

	void SetSpecular(const Vector3& specular){ mData.mSpecular = specular; }

	void SetPosition(const Vector3& position) { mData.mPosition = position; }

	void SetSpecularPower(float specular_power){ mData.mSpecularPower = specular_power; }

	void SetInnerRadius(float inner_radius){ mData.mInnerRadius = inner_radius; }

	void SetOuterRadius(float outer_radius){ mData.mOuterRadius = outer_radius; }

	void SetEnabled(bool enabled){ mData.mEnabled = enabled; }

	Vector3 GetDiffuse() const{ return mData.mDiffuse; }

	Vector3 GetSpecular() const { return mData.mSpecular; }

	Vector3 GetPosition() const{ return mData.mPosition; }

	float GetSpecularPower() const{ return mData.mSpecularPower; }

	float GetInnerRadius() const{ return mData.mInnerRadius; }

	float GetOuterRadius() const{ return mData.mOuterRadius; }

	bool GetEnabled() const{ return mData.mEnabled; }

	void SetProperties(const rapidjson::Value& properties) override;
private:
	PointLightData mData;
};

DECL_PTR(PointLightComponent);
