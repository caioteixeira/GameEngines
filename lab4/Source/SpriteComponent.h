// SpriteComponent.h
// 2D sprite-based draw component

#pragma once
#include "DrawComponent.h"
#include "Texture.h"

class SpriteComponent : public DrawComponent
{
	DECL_COMPONENT(SpriteComponent, DrawComponent);
public:
	SpriteComponent(Actor& owner);

	void Draw(class Renderer& render) override;

	void SetTexture(TexturePtr texture) { mTexture = texture; }
	TexturePtr GetTexture() { return mTexture; }
private:
	TexturePtr mTexture;
};

DECL_PTR(SpriteComponent);
