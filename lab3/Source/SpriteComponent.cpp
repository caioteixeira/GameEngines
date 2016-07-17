#include "ITPEnginePCH.h"
#include <SDL/SDL.h>

IMPL_COMPONENT(SpriteComponent, DrawComponent, 100);

SpriteComponent::SpriteComponent(Actor& owner)
	:DrawComponent(owner)
{

}

void SpriteComponent::Draw(Renderer& render)
{
	// For sprites, we need an additional scale matrix for the quad
	if (mTexture)
	{
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(mTexture->GetWidth()),
			static_cast<float>(mTexture->GetHeight()),
			1.0f);
		Matrix4 worldTrans = scaleMat * mOwner.GetWorldTransform();
		render.DrawSprite(mTexture, worldTrans);
	}
}

