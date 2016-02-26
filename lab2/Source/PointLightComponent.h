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

	// TODO: Lab 3
	// Add getter/setter functions for PointLightData

	void SetProperties(const rapidjson::Value& properties) override;
private:
	PointLightData mData;
};

DECL_PTR(PointLightComponent);
