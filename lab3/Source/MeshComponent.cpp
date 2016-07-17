#include "ITPEnginePCH.h"

IMPL_COMPONENT(MeshComponent, DrawComponent, 1024);

MeshComponent::MeshComponent(Actor& owner)
	:Super(owner)
	,mTextureIndex(0)
{

}

void MeshComponent::Draw(class Renderer& render)
{
	if (mMesh)
	{
		render.DrawMesh(mMesh->GetVertexArray(), mMesh->GetTexture(mTextureIndex),
			mOwner.GetWorldTransform(), mMesh->GetShaderType());
	}
}

void MeshComponent::SetProperties(const rapidjson::Value& properties)
{
	Super::SetProperties(properties);

	int textureIndex;
	std::string meshName;

	if(GetStringFromJSON(properties, "mesh", meshName))
	{
		auto mesh = mOwner.GetGame().GetAssetCache().Load<Mesh>(meshName);

		SetMesh(mesh);

		if(GetIntFromJSON(properties, "textureIndex", textureIndex))
		{
			SetTextureIndex(textureIndex);
		}
	}
}
