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

	// Set the lighting constant buffer
	mGraphicsDriver.SetPSConstantBuffer(mLightingBuffer, 2);

	if (mMatrixPaletteBuffer != nullptr)
	{
		mGraphicsDriver.SetVSConstantBuffer(mMatrixPaletteBuffer, 3);
	}
}

void Shader::CreateMatrixPaletteBuffer()
{
	mMatrixPaletteBuffer = mGraphicsDriver.CreateGraphicsBuffer(nullptr, sizeof(MatrixPalette), EBF_ConstantBuffer, ECPUAF_CanWrite, EGBU_Dynamic);
	mGraphicsDriver.SetVSConstantBuffer(mMatrixPaletteBuffer, 3);
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
	void *buffer = mGraphicsDriver.MapBuffer(mLightingBuffer);
	memcpy(buffer, &mLighting, sizeof(LightingConstants));
	mGraphicsDriver.UnmapBuffer(mLightingBuffer);
}

void Shader::UploadMatrixPalette(const struct MatrixPalette& palette)
{
	void *buffer = mGraphicsDriver.MapBuffer(mMatrixPaletteBuffer);
	memcpy(buffer, &palette, sizeof(MatrixPalette));
	mGraphicsDriver.UnmapBuffer(mMatrixPaletteBuffer);
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
	mLightingBuffer = mGraphicsDriver.CreateGraphicsBuffer(nullptr, sizeof(LightingConstants), EBF_ConstantBuffer, ECPUAF_CanWrite, EGBU_Dynamic);

	//Create sampler
	mDefaultSampler = mGraphicsDriver.CreateSamplerState();
	mGraphicsDriver.SetPSSamplerState(mDefaultSampler, 0);

	return mPixelShader != nullptr && mVertexShader != nullptr;
}
