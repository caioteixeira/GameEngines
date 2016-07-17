#pragma once
#include "Asset.h"
#include "VertexArray.h"
#include "Texture.h"
#include <vector>
#include "ShaderTypes.h"
#include "CollisionHelpers.h"

class Mesh : public Asset
{
	DECL_ASSET(Mesh, Asset);
public:
	Mesh(class Game& game);
	virtual ~Mesh();

	bool Load(const char* fileName, class AssetCache* cache) override;

	VertexArrayPtr GetVertexArray() { return mVertexArray; }
	TexturePtr GetTexture(size_t index);

	const Collision::Sphere& GetBoundingSphere() const { return mBoundingSphere; }
	const Collision::AxisAlignedBox& GetBoundingBox() const { return mBoundingBox; }

	EMeshShader GetShaderType() const { return mShaderType; }
private:
	Collision::Sphere mBoundingSphere;
	Collision::AxisAlignedBox mBoundingBox;
	VertexArrayPtr mVertexArray;
	std::vector<TexturePtr> mTextures;
	EMeshShader mShaderType;
};

DECL_PTR(Mesh);
