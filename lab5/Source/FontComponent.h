#pragma once
#include "DrawComponent.h"
#include "Texture.h"
#include <string>
#include "Math.h"
#include "Font.h"

class FontComponent : public DrawComponent
{
	DECL_COMPONENT(FontComponent, DrawComponent);
public:
	enum Alignment
	{
		AlignLeft,
		AlignCenter,
		AlignRight
	};

	FontComponent(Actor& owner);

	void Draw(class Renderer& render) override;

	void SetFont(FontPtr font) { mFont = font; }

	void SetText(const std::string& text, const Vector3& color, int pointSize = 24);

	void SetAlignment(Alignment align) { mAlignment = align; }

	void ClearText();
private:
	TexturePtr mTexture;
	FontPtr mFont;
	Alignment mAlignment;
};

DECL_PTR(FontComponent);
