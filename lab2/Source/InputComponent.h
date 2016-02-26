#pragma once
#include "MoveComponent.h"
#include <string>

class InputComponent : public MoveComponent
{
	DECL_COMPONENT(InputComponent, MoveComponent);
public:
	InputComponent(Actor& owner);

	void BindLinearAxis(const std::string& name);
	void BindYawAxis(const std::string& name);
	void BindPitchAxis(const std::string& name);
	void BindHorizontalAxis(const std::string& name);
	
	void OnLinearAxis(float value);
	void OnYawAxis(float value);
	void OnPitchAxis(float value);
	void OnHorizontalAxis(float value);
};

DECL_PTR(InputComponent);
