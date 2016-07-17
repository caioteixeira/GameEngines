#pragma once
#include "DrawComponent.h"
#include "Mesh.h"

class MeshComponent : public DrawComponent
{
	DECL_COMPONENT(MeshComponent, DrawComponent);
public:
	MeshComponent(Actor& owner);

	void Draw(class Renderer& render) override;

	void SetMesh(MeshPtr mesh) { mMesh = mesh; }
	MeshPtr GetMesh() { return mMesh; }
	void SetTextureIndex(int idx) { mTextureIndex = idx; }

	void SetProperties(const rapidjson::Value& properties) override;
protected:
	MeshPtr mMesh;
	int mTextureIndex;
};

DECL_PTR(MeshComponent);
