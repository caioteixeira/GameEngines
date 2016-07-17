#pragma once
#include "Mesh.h"
#include "MatrixPalette.h"

class RenderCommand
{
public:
	RenderCommand() {}

	virtual ~RenderCommand()
	{
	}

	virtual void Run(class Renderer& render) {}
};

class DrawMeshCommand : public RenderCommand
{
public:
	DrawMeshCommand(MeshPtr mesh, Matrix4 worldTransform, int textureIndex);
	void Run(class Renderer& render) override;
private:
	MeshPtr mMesh;
	Matrix4 mWorldTransform;
	int mTextureIndex;
};

class DrawSkinnedMeshCommand : public RenderCommand
{
public:
	DrawSkinnedMeshCommand(MeshPtr mesh, Matrix4 worldTransform, int textureIndex, MatrixPalette palette);
	void Run(class Renderer& render) override;
private:
	MeshPtr mMesh;
	Matrix4 mWorldTransform;
	int mTextureIndex;
	MatrixPalette mPalette;
};