#include "ITPEnginePCH.h"

IMPL_COMPONENT(InputComponent, MoveComponent, 8);

InputComponent::InputComponent(Actor& owner)
	:Super(owner)
{

}

void InputComponent::BindLinearAxis(const std::string& name)
{
	mOwner.GetGame().GetInput().BindAxis(name, this,
		&InputComponent::OnLinearAxis);
}

void InputComponent::BindYawAxis(const std::string& name)
{
	mOwner.GetGame().GetInput().BindAxis(name, this,
		&InputComponent::OnYawAxis);
}

void InputComponent::BindPitchAxis(const std::string& name)
{
	mOwner.GetGame().GetInput().BindAxis(name, this,
		&InputComponent::OnPitchAxis);
}

void InputComponent::BindHorizontalAxis(const std::string& name)
{
	mOwner.GetGame().GetInput().BindAxis(name, this,
		&InputComponent::OnHorizontalAxis);
}

void InputComponent::OnLinearAxis(float value)
{
	SetLinearAxis(value);
}

void InputComponent::OnYawAxis(float value)
{
	SetAngularAxis(value);
}

void InputComponent::OnPitchAxis(float value)
{
	SetPitchAxis(value);
}

void InputComponent::OnHorizontalAxis(float value)
{
	SetHorizontalAxis(value);
}
