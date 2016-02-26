// Render.h
// Tracks all the active draw components and renders
// the scene

#pragma once
#include <SDL/SDL.h>
#include <unordered_set>
#include "DrawComponent.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "CameraComponent.h"
#include "GraphicsDriver.h"
#include "InputLayoutCache.h"
#include "PointLightComponent.h"
#include "ShaderTypes.h"

class Renderer
{
public:
	Renderer(class Game& game);
	~Renderer();

	bool Init(int width, int height);

	void RenderFrame();

	void AddComponent(DrawComponentPtr component);
	void RemoveComponent(DrawComponentPtr component);

	void AddPointLight(PointLightComponentPtr light);
	void RemovePointLight(PointLightComponentPtr light);

	int GetWidth() const { return mWidth; }
	int GetHeight() const { return mHeight; }

	void DrawSprite(TexturePtr texture, const Matrix4& worldTransform);
	void DrawMesh(VertexArrayPtr vertArray, TexturePtr texture, const Matrix4& worldTransform, EMeshShader type = EMS_Basic);
	void DrawSkeletalMesh(VertexArrayPtr vertArray, TexturePtr texture, const Matrix4& worldTransform, const struct MatrixPalette& palette);
	void DrawVertexArray(VertexArrayPtr vertArray);

	void UpdateViewMatrix(const Matrix4& view);
	void SetAmbientLight(const Vector3& color);

	// Given a screen space point, unprojects it into world space,
	// based on the current 3D view/projection matrices
	// Expected ranges:
	// x = [-screenWidth/2, +screenWidth/2]
	// y = [-screenHeight/2, +screenHeight/2]
	// z = [0, 1] -- 0 means near plane, 1 means far plane
	Vector3 Unproject(const Vector3& screenPoint) const;

	GraphicsDriver& GetGraphicsDriver() { return *mGraphicsDriver; }
	InputLayoutCache& GetInputLayoutCache() { return *mInputLayoutCache; }
private:
	void Clear();
	void DrawComponents();
	void Present();

	bool InitFrameBuffer();
	bool InitShaders();
	bool InitSpriteVerts();

	void UpdatePointLights();

	std::shared_ptr<GraphicsDriver> mGraphicsDriver;
	std::shared_ptr<InputLayoutCache> mInputLayoutCache;

	std::unordered_set<DrawComponentPtr> mComponents2D;
	std::unordered_set<DrawComponentPtr> mDrawComponents;

	std::unordered_set<PointLightComponentPtr> mPointLights;

	std::unordered_map<EMeshShader, ShaderPtr> mMeshShaders;

	// View-projection matrix
	Matrix4 mSpriteViewProj;
	// View matrix in 3D
	Matrix4 mView;
	// Projection matrix in 3D
	Matrix4 mProj;

	SDL_Window* mWindow;

	DepthStencilPtr mDepthBuffer;
	DepthStencilStatePtr mSpriteDepthState;
	DepthStencilStatePtr mMeshDepthState;

	RasterizerStatePtr mRasterState;

	BlendStatePtr mSpriteBlendState;
	BlendStatePtr mMeshBlendState;

	ShaderPtr mSpriteShader;
	VertexArrayPtr mSpriteVerts;

	class Game& mGame;

	int mWidth;
	int mHeight;
};
