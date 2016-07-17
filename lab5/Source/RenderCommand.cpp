#include "ITPEnginePCH.h"

DrawMeshCommand::DrawMeshCommand(MeshPtr mesh, Matrix4 worldTransform, int textureIndex)
	: mMesh(mesh), mWorldTransform(worldTransform), mTextureIndex(textureIndex)
{
}

void DrawMeshCommand::Run(Renderer& render)
{
	render.DrawMesh(mMesh->GetVertexArray(), mMesh->GetTexture(mTextureIndex),
		mWorldTransform, mMesh->GetShaderType());
}

DrawSkinnedMeshCommand::DrawSkinnedMeshCommand(MeshPtr mesh, Matrix4 worldTransform, int textureIndex, MatrixPalette palette)
	: mMesh(mesh), mWorldTransform(worldTransform), mTextureIndex(textureIndex), mPalette(palette)
{
}

void DrawSkinnedMeshCommand::Run(Renderer& render)
{
	render.DrawSkeletalMesh(mMesh->GetVertexArray(), mMesh->GetTexture(mTextureIndex),
		mWorldTransform, mPalette);
}
