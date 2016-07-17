#include "ITPEnginePCH.h"

IMPL_COMPONENT(FontComponent, DrawComponent, 100);

FontComponent::FontComponent(Actor& owner)
	:Super(owner)
	,mAlignment(AlignLeft)
{

}

void FontComponent::Draw(class Renderer& render)
{
	if (mTexture)
	{
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(mTexture->GetWidth()),
			static_cast<float>(mTexture->GetHeight()),
			1.0f);
		Matrix4 worldTrans;
		switch (mAlignment)
		{
		case AlignCenter:
			worldTrans = scaleMat * mOwner.GetWorldTransform();
			break;
		case AlignRight:
			worldTrans = scaleMat * Matrix4::CreateTranslation(
				Vector3(mTexture->GetWidth() / -2.0f,
				0.0f,
				0.0f)) *
				mOwner.GetWorldTransform();
			break;
		case AlignLeft:
			worldTrans = scaleMat * Matrix4::CreateTranslation(
				Vector3(mTexture->GetWidth() / 2.0f,
				0.0f,
				0.0f)) *
				mOwner.GetWorldTransform();
		default:
			break;
		}
		render.DrawSprite(mTexture, worldTrans);
	}
}

void FontComponent::SetText(const std::string& text, const Vector3& color, int pointSize /*= 24*/)
{
	// Convert to SDL_Color
	SDL_Color sdlColor;
	sdlColor.r = static_cast<Uint8>(color.x * 255);
	sdlColor.g = static_cast<Uint8>(color.y * 255);
	sdlColor.b = static_cast<Uint8>(color.z * 255);
	sdlColor.a = 255;

	TTF_Font* font = mFont->GetFontData(pointSize);
	if (font)
	{
		SDL_Surface* surf = TTF_RenderText_Blended(font, text.c_str(), sdlColor);
		if (surf != nullptr)
		{
			mTexture = Texture::CreateFromSurface(mOwner.GetGame(), surf);
			SDL_FreeSurface(surf);
		}
		else
		{
			mTexture.reset();
		}
	}
	else
	{
		SDL_Log("Point size %d is unsupported", pointSize);
	}
}

void FontComponent::ClearText()
{
	mTexture.reset();
}
