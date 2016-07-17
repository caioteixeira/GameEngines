// DrawComponent.h
// Base class for all components that can be drawn by Renderer

#pragma once
#include "Component.h"
#include "RenderCommand.h"
#include <queue>

class DrawComponent : public Component
{
	DECL_COMPONENT(DrawComponent, Component);
public:
	DrawComponent(Actor& owner);

	void Register() override;
	void Unregister() override;
	
	virtual void Draw(class Renderer& render);

	virtual void Extract(std::queue<std::shared_ptr<RenderCommand>> & updateQueue);

	void SetIsVisible(bool value) { mIsVisible = value; }
	bool IsVisible() const { return mIsVisible; }
private:
	bool mIsVisible;
};

DECL_PTR(DrawComponent);
