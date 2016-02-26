#include "ITPEnginePCH.h"
#include <SDL/SDL_log.h>
#include <vector>

Shader::Shader(class Game& game)
	:Asset(game)
	,mGraphicsDriver(mGame.GetRenderer().GetGraphicsDriver())
{
}

Shader::~Shader()
{
}

void Shader::SetActive()
{
	// Set this shader as active, and set
	// any constant buffers
	mGraphicsDriver.SetVertexShader(mVertexShader);
	mGraphicsDriver.SetPixelShader(mPixelShader);

	mGraphicsDriver.SetVSConstantBuffer(mPerCameraBuffer, 0);
	mGraphicsDriver.SetVSConstantBuffer(mPerObjectBuffer, 1);
	mGraphicsDriver.SetPSConstantBuffer(mPerCameraBuffer, 0);
	mGraphicsDriver.SetPSConstantBuffer(mPerObjectBuffer, 1);
	
	mGraphicsDriver.SetPSSamplerState(mDefaultSampler, 0);

	// TODO: Lab 3
	// Set the lighting constant buffer

	// TODO: Lab 4
	// Set the matrix palette buffer, if it exists
}

void Shader::CreateMatrixPaletteBuffer()
{
	// TODO: Lab 4
}

void Shader::UploadPerCameraConstants()
{
	void *camera_buffer = mGraphicsDriver.MapBuffer(mPerCameraBuffer);
	memcpy(camera_buffer, &mPerCamera, sizeof(PerCameraConstants));
	mGraphicsDriver.UnmapBuffer(mPerCameraBuffer);
}

void Shader::UploadPerObjectConstants()
{
	void *buffer = mGraphicsDriver.MapBuffer(mPerObjectBuffer);
	memcpy(buffer, &mPerObject, sizeof(PerObjectConstants));
	mGraphicsDriver.UnmapBuffer(mPerObjectBuffer);
}

void Shader::UploadLightingConstants()
{
	// TODO: Lab 3
}

void Shader::UploadMatrixPalette(const struct MatrixPalette& palette)
{
	// TODO: Lab 4
}

void Shader::BindTexture(TexturePtr texture, int slot)
{
	texture->SetActive(slot);
}

bool Shader::Load(const char* fileName, class AssetCache* cache)
{
	std::vector<char> temp;

	//Compile and load vertex shader
	mGraphicsDriver.CompileShaderFromFile(fileName, "VS", "vs_4_0", temp);

	mVertexShader = mGraphicsDriver.CreateVertexShader(temp);
	mCompiledVS = temp;

	temp.clear();

	//Compile and load pixel shader
	mGraphicsDriver.CompileShaderFromFile(fileName, "PS", "ps_4_0", temp);
	mPixelShader = mGraphicsDriver.CreatePixelShader(temp);
	mCompiledPS = temp;

	//Create constant buffers
	mPerCameraBuffer = mGraphicsDriver.CreateGraphicsBuffer(nullptr, sizeof(PerCameraConstants), EBF_ConstantBuffer, ECPUAF_CanWrite, EGBU_Dynamic);
	mPerObjectBuffer = mGraphicsDriver.CreateGraphicsBuffer(nullptr, sizeof(PerObjectConstants), EBF_ConstantBuffer, ECPUAF_CanWrite, EGBU_Dynamic);

	//Create sampler
	mDefaultSampler = mGraphicsDriver.CreateSamplerState();
	mGraphicsDriver.SetPSSamplerState(mDefaultSampler, 0);

	return mPixelShader != nullptr && mVertexShader != nullptr;
}
