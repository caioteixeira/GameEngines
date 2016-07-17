#include "ITPEnginePCH.h"

Renderer::Renderer(Game& game)
	:mGame(game)
	,mWindow(nullptr)
	,mWidth(0)
	,mHeight(0)
{

}

Renderer::~Renderer()
{
	// Clear components...
	mDrawComponents.clear();
	mComponents2D.clear();

	mDepthBuffer.reset();
	mSpriteDepthState.reset();
	mMeshDepthState.reset();
	mRasterState.reset();

	mSpriteBlendState.reset();
	mMeshBlendState.reset();

	mSpriteShader.reset();
	mSpriteVerts.reset();

	mMeshShaders.clear();

	// Shutdown the input cache and graphics driver
	mInputLayoutCache.reset();
	mGraphicsDriver.reset();

	if (mWindow != nullptr)
	{
		SDL_DestroyWindow(mWindow);
	}
}

bool Renderer::Init(int width, int height)
{
	// Create our SDL window
	mWindow = SDL_CreateWindow("ITP Engine 2 Demo!", 100, 100, width, height, 
		0);

	if (!mWindow)
	{
		SDL_Log("Could not create window.");
		return false;
	}

	mGraphicsDriver = std::make_shared<GraphicsDriver>(GetActiveWindow());
	mInputLayoutCache = std::make_shared<InputLayoutCache>();

	mWidth = width;
	mHeight = height;

	if (!InitFrameBuffer())
	{
		return false;
	}

	if (!InitShaders())
	{
		return false;
	}

	if (!InitSpriteVerts())
	{
		return false;
	}

	return true;
}

void Renderer::RenderFrame()
{
	Clear();
	DrawComponents();
	Present();
}

void Renderer::AddComponent(DrawComponentPtr component)
{
	if (IsA<SpriteComponent>(component) || IsA<FontComponent>(component))
	{
		mComponents2D.emplace(component);
	}
	else
	{
		mDrawComponents.emplace(component);
	}
}

void Renderer::RemoveComponent(DrawComponentPtr component)
{
	if (IsA<SpriteComponent>(component) || IsA<FontComponent>(component))
	{
		auto iter = mComponents2D.find(component);
		if (iter != mComponents2D.end())
		{
			mComponents2D.erase(iter);
		}
	}
	else
	{
		auto iter = mDrawComponents.find(component);
		if (iter != mDrawComponents.end())
		{
			mDrawComponents.erase(iter);
		}
	}
}

void Renderer::AddPointLight(PointLightComponentPtr light)
{
	mPointLights.emplace(light);
}

void Renderer::RemovePointLight(PointLightComponentPtr light)
{
	mPointLights.erase(light);
}

void Renderer::UpdatePointLights()
{
	for (auto & shader : mMeshShaders)
	{
		unsigned int i = 0;
		for(auto & light : mPointLights)
		{
			PointLightData * data = &shader.second->GetLightingConstants().pointLights[i];
			data->mDiffuse = light->GetDiffuse();
			data->mSpecular = light->GetSpecular();
			data->mEnabled = light->GetEnabled();
			data->mInnerRadius = light->GetInnerRadius();
			data->mOuterRadius = light->GetOuterRadius();
			data->mPosition = light->GetPosition();
			data->mSpecularPower = light->GetSpecularPower();
			i++;
		}

		while(i < 8)
		{
			shader.second->GetLightingConstants().pointLights[i].mEnabled = false;
			i++;
		}

		shader.second->UploadLightingConstants();
	}
}

void Renderer::DrawSprite(TexturePtr texture, const Matrix4& worldTransform)
{
	mSpriteShader->SetActive();
	mSpriteShader->GetPerObjectConstants().worldMatrix = worldTransform;
	mSpriteShader->UploadPerObjectConstants();

	mSpriteShader->BindTexture(texture, 0);

	DrawVertexArray(mSpriteVerts);
}

void Renderer::DrawMesh(VertexArrayPtr vertArray, TexturePtr texture, const Matrix4& worldTransform, EMeshShader type /*= EMS_Basic*/)
{
	auto & shader = mMeshShaders[type];
	
	shader->SetActive();
	shader->GetPerObjectConstants().worldMatrix = worldTransform;
	shader->UploadPerObjectConstants();
	shader->BindTexture(texture, 0);

	DrawVertexArray(vertArray);
	
}

void Renderer::DrawSkeletalMesh(VertexArrayPtr vertArray, TexturePtr texture, const Matrix4& worldTransform, const struct MatrixPalette& palette)
{
	// TODO: Lab 4
}

void Renderer::DrawVertexArray(VertexArrayPtr vertArray)
{
	vertArray->SetActive();
	mGraphicsDriver->DrawIndexed(vertArray->GetIndexCount(), 0, 0);
}

void Renderer::UpdateViewMatrix(const Matrix4& view)
{
	mView = view;
	for(auto & shader : mMeshShaders)
	{
		shader.second->GetPerCameraConstants().projMatrix = mView * mProj;
		shader.second->GetPerCameraConstants().cameraWorldPos = mView.GetTranslation();
	}
}

void Renderer::SetAmbientLight(const Vector3& color)
{
	for(auto & shader : mMeshShaders)
	{
		shader.second->GetLightingConstants().ambientLight = color;
		shader.second->UploadLightingConstants();
	}
}

Vector3 Renderer::Unproject(const Vector3& screenPoint) const
{
	// Convert screenPoint to device coordinates (between -1 and +1)
	Vector3 deviceCoord = screenPoint;
	deviceCoord.x /= (mWidth) * 0.5f;
	deviceCoord.y /= (mHeight) * 0.5f;

	// First, undo the projection
	Matrix4 unprojection = mProj;
	unprojection.Invert();
	Vector3 unprojVec = TransformWithPerspDiv(deviceCoord, unprojection);

	// Now undo the view matrix
	Matrix4 uncamera = mView;
	uncamera.Invert();
	return Transform(unprojVec, uncamera);
}

void Renderer::Clear()
{
	mGraphicsDriver->ClearBackBuffer(Vector3(0.0f, 0.0f, 0.0f), 1.0f);
	mGraphicsDriver->ClearDepthStencil(mDepthBuffer, 1.0f);
}

void Renderer::DrawComponents()
{

	// Upload per camera constants
	for(auto & shader : mMeshShaders)
	{
		shader.second->UploadPerCameraConstants();
	}

	// Update point lights
	UpdatePointLights();

	// Enable depth buffering and disable blending
	mGraphicsDriver->SetDepthStencilState(mMeshDepthState);
	mGraphicsDriver->SetBlendState(mMeshBlendState);
	// Draw the normal components
	for (auto& comp : mDrawComponents)
	{
		if (comp->IsVisible())
		{
			comp->Draw(*this);
		}
	}

	// Disable depth buffering and enable blending	
	mGraphicsDriver->SetDepthStencilState(mSpriteDepthState);
	mGraphicsDriver->SetBlendState(mSpriteBlendState);
	// Draw the 2D components
	for (auto& comp : mComponents2D)
	{
		if (comp->IsVisible())
		{
			comp->Draw(*this);
		}
	}
}

void Renderer::Present()
{
	mGraphicsDriver->Present();
}

bool Renderer::InitFrameBuffer()
{
	auto rasterizer_state = mGraphicsDriver->CreateRasterizerState(EFillMode::EFM_Solid);
	mGraphicsDriver->SetRasterizerState(rasterizer_state);

	mDepthBuffer = mGraphicsDriver->CreateDepthStencil(mWidth, mHeight);
	mGraphicsDriver->SetDepthStencil(mDepthBuffer);

	mMeshDepthState = mGraphicsDriver->CreateDepthStencilState(true, EComparisonFunc::ECF_Less);
	mSpriteDepthState = mGraphicsDriver->CreateDepthStencilState(false, EComparisonFunc::ECF_LessEqual);

	mMeshBlendState = mGraphicsDriver->CreateBlendState(false);
	mSpriteBlendState = mGraphicsDriver->CreateBlendState(true);

	return true;
}

bool Renderer::InitShaders()
{
	// Load sprite shader and basic shader
	mSpriteShader = mGame.GetAssetCache().Load<Shader>("Shaders/Sprite.hlsl");
	if (mSpriteShader == nullptr)
		return false;

	//Basic mesh setup
	mProj = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f), static_cast<float>(mWidth), static_cast<float>(mHeight), 25.0f, 10000.0f);

	auto basic_mesh_shader = mGame.GetAssetCache().Load<Shader>("Shaders/BasicMesh.hlsl");
	if (basic_mesh_shader == nullptr)
		return false;
	mMeshShaders.emplace(EMS_Basic, basic_mesh_shader);
	InputLayoutElement vs_mesh_elements[] = {
		InputLayoutElement("POSITION", 0, EGF_R32G32B32_Float, 0),
		InputLayoutElement("NORMAL", 0, EGF_R32G32B32_Float, 12),
		InputLayoutElement("TEXCOORD", 0, EGF_R32G32_Float, 24),
	};
	auto vs_mesh_layout = mGraphicsDriver->CreateInputLayout(vs_mesh_elements, 3, mMeshShaders[EMS_Basic]->GetCompiledVS());
	mInputLayoutCache->RegisterLayout("positionnormaltexcoord", vs_mesh_layout);

	// Sprite 
	//Vertex shader
	InputLayoutElement vs_elements[] = {
		InputLayoutElement("POSITION", 0, EGF_R32G32B32_Float, 0),
		InputLayoutElement("TEXCOORD", 0, EGF_R32G32_Float, 12),
	};
	auto vs_inputLayout = mGraphicsDriver->CreateInputLayout(vs_elements, 2, mSpriteShader->GetCompiledVS());
	mInputLayoutCache->RegisterLayout("positiontexcoord", vs_inputLayout);

	//Set up view-projection matrix
	mSpriteShader->GetPerCameraConstants().projMatrix = Matrix4::CreateOrtho(static_cast<float>(mWidth), static_cast<float>(mHeight), 1000.0f, -1000.0f);
	mSpriteShader->UploadPerCameraConstants();
	
	// Load phong shader
	mProj = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f), static_cast<float>(mWidth), static_cast<float>(mHeight), 25.0f, 10000.0f);

	auto phongShader = mGame.GetAssetCache().Load<Shader>("Shaders/Phong.hlsl");
	if (phongShader == nullptr)
		return false;
	mMeshShaders.emplace(EMS_Phong, phongShader);
	InputLayoutElement vsPhongElements[] = {
		InputLayoutElement("POSITION", 0, EGF_R32G32B32_Float, 0),
		InputLayoutElement("NORMAL", 0, EGF_R32G32B32_Float, 12),
		InputLayoutElement("TEXCOORD", 0, EGF_R32G32_Float, 24),
	};
	auto vsPhongLayout = mGraphicsDriver->CreateInputLayout(vs_mesh_elements, 3, mMeshShaders[EMS_Phong]->GetCompiledVS());
	mInputLayoutCache->RegisterLayout("positionnormaltexcoord", vsPhongLayout);

	// TODO: Lab 4
	// Load skinned shader and create appropriate input layout

	return true;
}

bool Renderer::InitSpriteVerts()
{
	// Create the vertex array for sprites
	float verts[] =
	{
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f,  // top left
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f,   // top right
		0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom left
	};

	uint16_t indices[] =
	{
		0, 1, 2, // <top left, top right, bottom right>
		2, 3, 0, // <bottom right, bottom left, top left>
	};

	mSpriteVerts = VertexArray::Create(GetGraphicsDriver(), GetInputLayoutCache(),
		verts, 4, 20, "positiontexcoord", indices, 6);

	return true;
}
