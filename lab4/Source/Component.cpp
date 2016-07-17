#include "ITPEnginePCH.h"

IMPL_COMPONENT(Component, Object, 1);

Component::Component(Actor& owner)
	:mOwner(owner)
{
}

Component::~Component()
{
	mOwner.GetGame().GetGameTimers().ClearAllTimers(this);
}

void Component::Tick(float deltaTime)
{

}

void Component::Register()
{

}

void Component::Unregister()
{
}

void Component::OnUpdatedTransform()
{

}
