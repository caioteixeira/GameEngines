// We want to use row major matrices
#pragma pack_matrix(row_major)

cbuffer PER_CAMERA : register(b0)
{
	float4x4 projMatrix;
}

cbuffer PER_OBJECT : register(b1)
{
	float4x4 worldMatrix;
}

SamplerState DefaultSampler : register(s0);
Texture2D DiffuseTexture : register(t0);